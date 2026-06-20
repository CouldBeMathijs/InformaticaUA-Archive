def get_lib_asm():
    return """
.text
malloc:
# $a0: size
li $t0, 32          # Minimum data size of 32 bytes
bge $a0, $t0, malloc_min_ok
move $a0, $t0
malloc_min_ok:
addiu $a0, $a0, 7
li $t0, -8
and $a0, $a0, $t0   # align requested size to 8
addiu $a0, $a0, 8   # add 8 bytes for header
li $v0, 9           # sbrk syscall
syscall
# $v0 has the address of the header
addiu $t0, $a0, -8  # get the aligned size back
sw $t0, ($v0)       # store size in first 4 bytes of header
addiu $v0, $v0, 8   # return address after 8-byte header
jr $ra

free:
jr $ra              # no-op free

calloc:
# $a0: nmemb, $a1: size
mul $a0, $a0, $a1
move $t0, $a0       # save total size

# Save $ra and $t0
addiu $sp, $sp, -8
sw $ra, 4($sp)
sw $t0, 0($sp)

jal malloc

lw $t0, 0($sp)
lw $ra, 4($sp)
addiu $sp, $sp, 8

# $v0 has the pointer, $t0 has the size
move $t1, $v0       # current ptr
addu $t2, $v0, $t0  # end ptr
calloc_loop:
bge $t1, $t2, calloc_done
sb $zero, ($t1)
addiu $t1, $t1, 1
j calloc_loop
calloc_done:
jr $ra

realloc:
# $a0: old_ptr, $a1: new_size
beqz $a0, realloc_malloc
# --- SANITY GUARD: Handle invalid pointer in realloc ---
li $t0, 0x00400000
blt $a0, $t0, realloc_malloc

# Get old size from header
lw $t0, -8($a0)

# IF new_size <= old_size, don't shrink it! Just return the original pointer.
ble $a1, $t0, realloc_keep_old

# Save regs
addiu $sp, $sp, -16
sw $ra, 12($sp)
sw $s0, 8($sp)
sw $s1, 4($sp)
sw $s2, 0($sp)

move $s0, $a0       # $s0 = old_ptr
move $s1, $a1       # $s1 = new_size

# Allocate new larger block
move $a0, $s1
jal malloc
move $s2, $v0       # $s2 = new_ptr

# Copy content
lw $a2, -8($s0)     # copy_size = old_size
move $a0, $s2       # dest = new_ptr
move $a1, $s0       # src = old_ptr
jal memcpy

move $v0, $s2       # return new_ptr

# Restore regs
lw $s2, 0($sp)
lw $s1, 4($sp)
lw $s0, 8($sp)
lw $ra, 12($sp)
addiu $sp, $sp, 16
jr $ra

realloc_keep_old:
move $v0, $a0       # return old_ptr unchanged
jr $ra

realloc_malloc:
move $a0, $a1
j malloc

memcpy:
# $a0: dest, $a1: src, $a2: n
move $v0, $a0       # return dest
# --- SANITY GUARD: Check if dest and src pointers are valid ---
li $t0, 0x00400000
blt $a0, $t0, memcpy_done
blt $a1, $t0, memcpy_done

move $t3, $a0       # Copy dest pointer to temporary $t3
move $t4, $a1       # Copy src pointer to temporary $t4
addu $t0, $t3, $a2  # end dest boundary

memcpy_loop:
bge $t3, $t0, memcpy_done
lb $t1, ($t4)
sb $t1, ($t3)
addiu $t3, $t3, 1   # Increment the temporary copy, leaving $a0 clean
addiu $t4, $t4, 1   # Increment the temporary copy, leaving $a1 clean
j memcpy_loop
memcpy_done:
jr $ra

fopen:
# $a0: filename, $a1: mode string
# --- SANITY GUARD: Check if filename/mode pointers are valid ---
li $t0, 0x00400000
blt $a0, $t0, fopen_fail
blt $a1, $t0, fopen_fail

lb $t0, ($a1)
li $t1, 'r'
beq $t0, $t1, fopen_read
li $t1, 'w'
beq $t0, $t1, fopen_write
fopen_fail:
li $v0, 0           # return NULL if unknown mode or bad address
jr $ra

fopen_read:
li $a1, 0           # flags: O_RDONLY
li $a2, 0           # mode: 0
li $v0, 13          # open syscall
syscall
# Note: SPIM returns -1 on error, C returns 0
bgez $v0, fopen_read_done
li $v0, 0
fopen_read_done:
jr $ra

fopen_write:
# Use 1 for write-only in SPIM/MARS (automatically handles O_CREAT | O_TRUNC)
li $a1, 1
li $a2, 0x1ff       # mode: 777
li $v0, 13
syscall
bgez $v0, fopen_write_done
li $v0, 0
fopen_write_done:
jr $ra

fclose:
# $a0: file descriptor
bnez $a0, fclose_proceed
li $v0, 10          # Crash/Exit immediately if file handle is NULL
syscall
fclose_proceed:
li $v0, 16          # close syscall
syscall
jr $ra

fputs:
# $a0: string, $a1: file descriptor
# --- SANITY GUARD: Check if string pointer is valid ---
li $t0, 0x00400000
blt $a0, $t0, fputs_fail

bnez $a1, fputs_proceed
li $v0, 10          # Crash/Exit immediately if file handle is NULL
syscall
fputs_proceed:
move $t0, $a0       # $t0: buffer
move $t1, $a1       # $t1: fd

# Calculate length
move $t2, $t0
fputs_len_loop:
lb $t3, ($t2)
beq $t3, $zero, fputs_len_done
addiu $t2, $t2, 1
j fputs_len_loop

fputs_len_done:
subu $a2, $t2, $t0  # $a2: length
move $a0, $t1       # $a0: fd
move $a1, $t0       # $a1: buffer
li $v0, 15          # write syscall
syscall
fputs_fail:
jr $ra

fgets:
# $a0: buffer, $a1: size, $a2: file descriptor
# --- SANITY GUARD: Check if destination buffer pointer is valid ---
li $t0, 0x00400000
blt $a0, $t0, fgets_null

bnez $a2, fgets_proceed
li $v0, 10          # Crash/Exit immediately if file handle is NULL
syscall
fgets_proceed:
# C fgets reads until newline or size-1.
move $t0, $a0       # $t0: buffer
move $t1, $a1       # $t1: size
move $t2, $a2       # $t2: fd
li $t3, 0           # $t3: count

fgets_loop:
addiu $t4, $t1, -1  # size - 1
bge $t3, $t4, fgets_end

move $a0, $t2       # fd
move $a1, $t0       # current buffer pos
li $a2, 1           # read 1 char
li $v0, 14          # read syscall
syscall

blez $v0, fgets_eof # EOF or error

lb $t4, ($t0)
addiu $t0, $t0, 1
addiu $t3, $t3, 1

li $t5, 10          # newline char
beq $t4, $t5, fgets_end
j fgets_loop

fgets_eof:
beqz $t3, fgets_null # if nothing read, return NULL
j fgets_end

fgets_null:
li $v0, 0
jr $ra

fgets_end:
sb $zero, ($t0)     # null terminate
move $v0, $a0       # return buffer pointer
jr $ra
"""


def get_printf_asm():
    return r"""
.text
puts:
li $v0, 4
syscall
li $a0, 10
li $v0, 11
syscall
li $v0, 0
jr $ra

printf:
li $t0, 0x00400000
bge $a0, $t0, printf_proceed
li $v0, 0
jr $ra

printf_proceed:
move $t0, $a0

printf_validate_loop:
lb $t2, ($t0)
beq $t2, $zero, printf_validate_ok
bne $t2, '%', printf_validate_next

printf_validate_percent:
addiu $t0, $t0, 1
lb $t2, ($t0)
beq $t2, $zero, printf_validate_fail

printf_validate_width:
beq $t2, 46, printf_validate_width_continue
li $t6, 48
blt $t2, $t6, printf_validate_type
li $t6, 57
bgt $t2, $t6, printf_validate_type
printf_validate_width_continue:
addiu $t0, $t0, 1
lb $t2, ($t0)
beq $t2, $zero, printf_validate_fail
j printf_validate_width

printf_validate_type:
beq $t2, 'd', printf_validate_next
beq $t2, 's', printf_validate_next
beq $t2, 'c', printf_validate_next
beq $t2, 'f', printf_validate_next
beq $t2, 'x', printf_validate_next
beq $t2, '%', printf_validate_next
j printf_validate_fail

printf_validate_next:
addiu $t0, $t0, 1
j printf_validate_loop

printf_validate_fail:
li $v0, -1
jr $ra

printf_validate_ok:
addiu $sp, $sp, -32
sw $ra, 28($sp)
sw $s0, 24($sp)
sw $a0, 32($sp)
sw $a1, 36($sp)
sw $a2, 40($sp)
sw $a3, 44($sp)

move $t0, $a0
addiu $t1, $sp, 36
li $s0, 0

printf_loop:
lb $t2, ($t0)
beq $t2, $zero, printf_end
beq $t2, '%', printf_parse_start

move $a0, $t2
li $v0, 11
syscall
addiu $s0, $s0, 1
addiu $t0, $t0, 1
j printf_loop

printf_parse_start:
addiu $t0, $t0, 1
lb $t2, ($t0)
li $t5, 0
li $t9, 6

printf_parse_width:
beq $t2, 46, printf_parse_dot
li $t6, 48
blt $t2, $t6, printf_check_type
li $t6, 57
bgt $t2, $t6, printf_check_type

mul $t5, $t5, 10
subu $t6, $t2, 48
addu $t5, $t5, $t6

addiu $t0, $t0, 1
lb $t2, ($t0)
j printf_parse_width

printf_parse_dot:
addiu $t0, $t0, 1
lb $t2, ($t0)
li $t9, 0

printf_parse_precision:
li $t6, 48
blt $t2, $t6, printf_check_type
li $t6, 57
bgt $t2, $t6, printf_check_type

mul $t9, $t9, 10
subu $t6, $t2, 48
addu $t9, $t9, $t6

addiu $t0, $t0, 1
lb $t2, ($t0)
j printf_parse_precision

printf_check_type:
beq $t2, 'd', printf_int
beq $t2, 's', printf_str
beq $t2, 'c', printf_char
beq $t2, 'f', printf_float
beq $t2, 'x', printf_hex
beq $t2, '%', printf_percent
j printf_next

printf_int:
lw $t4, ($t1)
move $t6, $t4
li $t7, 0

bnez $t6, printf_int_calc
li $t7, 1
j printf_pad

printf_int_calc:
bgez $t6, printf_int_loop
addiu $t7, $t7, 1
negu $t6, $t6

printf_int_loop:
beqz $t6, printf_pad
li $t8, 10
div $t6, $t8
mflo $t6
addiu $t7, $t7, 1
j printf_int_loop

printf_hex:
lw $t4, ($t1)
move $t6, $t4
li $t7, 0

bnez $t6, printf_hex_calc
li $t7, 1
j printf_pad

printf_hex_calc:
beqz $t6, printf_pad
srl $t6, $t6, 4
addiu $t7, $t7, 1
j printf_hex_calc

printf_float:
# Ensure the pointer $t1 is 8-byte aligned (ABI requirement for doubles)
# This aligns the current stack pointer $t1 to the nearest 8-byte boundary
addiu $t2, $t1, 4    # offset to check alignment
andi  $t2, $t2, 7    # check low 3 bits
bnez  $t2, printf_float_skip_align
addiu $t1, $t1, 4    # adjust pointer if misaligned

printf_float_skip_align:
# Correctly load the 64-bit double from the stack
lwc1 $f12, 0($t1)    # Load lower 32 bits
lwc1 $f13, 4($t1)    # Load upper 32 bits

printf_float_read:
lw $t2, 0($t1)
lw $t3, 4($t1)
mtc1 $t2, $f12
mtc1 $t3, $f13

bgez $t3, printf_float_pos
li $a0, '-'
li $v0, 11
syscall
addiu $s0, $s0, 1
neg.d $f12, $f12

printf_float_pos:
trunc.w.d $f0, $f12
mfc1 $t4, $f0
move $a0, $t4
li $v0, 1
syscall

move $t6, $t4
li $t7, 0
bnez $t6, printf_float_int_calc
li $t7, 1
j printf_float_int_done

printf_float_int_calc:
printf_float_int_loop:
beqz $t6, printf_float_int_done
li $t8, 10
div $t6, $t8
mflo $t6
addiu $t7, $t7, 1
j printf_float_int_loop

printf_float_int_done:
addu $s0, $s0, $t7
li $a0, '.'
li $v0, 11
syscall
addiu $s0, $s0, 1

cvt.d.w $f2, $f0
sub.d $f4, $f12, $f2

li $t4, 1
move $t6, $t9
beqz $t6, printf_float_pow_done
printf_float_pow_loop:
mul $t4, $t4, 10
addiu $t6, $t6, -1
bgtz $t6, printf_float_pow_loop
printf_float_pow_done:
mtc1 $t4, $f6
cvt.d.w $f6, $f6
mul.d $f4, $f4, $f6

li $t8, 1
mtc1 $t8, $f8
cvt.d.w $f8, $f8
li $t8, 2
mtc1 $t8, $f10
cvt.d.w $f10, $f10
div.d $f8, $f8, $f10
add.d $f4, $f4, $f8

trunc.w.d $f0, $f4
mfc1 $t4, $f0
bgez $t4, printf_float_frac_ready
li $t4, 0

printf_float_frac_ready:
move $t5, $t9
beqz $t5, printf_float_frac_skip

li $t6, 1
addiu $t2, $t9, -1
bltz $t2, printf_float_frac_skip
beqz $t2, printf_float_frac_loop
printf_float_div_loop:
mul $t6, $t6, 10
addiu $t2, $t2, -1
bgtz $t2, printf_float_div_loop

printf_float_frac_loop:
div $t4, $t6
mflo $a0
mfhi $t4
addiu $a0, $a0, 48
li $v0, 11
syscall
li $t7, 10
div $t6, $t7
mflo $t6
addiu $t5, $t5, -1
bgtz $t5, printf_float_frac_loop

printf_float_frac_skip:
addu $s0, $s0, $t9
addiu $t1, $t1, 8
j printf_next

printf_str:
lw $t4, ($t1)
li $t6, 0x00400000
bge $t4, $t6, printf_str_proceed
li $t7, 0
j printf_pad

printf_str_proceed:
move $t6, $t4
li $t7, 0
printf_str_loop:
lb $t8, ($t6)
beqz $t8, printf_pad
addiu $t6, $t6, 1
addiu $t7, $t7, 1
j printf_str_loop

printf_char:
lw $t4, ($t1)
li $t7, 1
j printf_pad

printf_percent:
li $a0, '%'
li $v0, 11
syscall
addiu $s0, $s0, 1
j printf_next

printf_pad:
subu $t8, $t5, $t7
blez $t8, printf_print_value

printf_pad_loop:
li $a0, 32
li $v0, 11
syscall
addiu $s0, $s0, 1
addiu $t8, $t8, -1
bgtz $t8, printf_pad_loop

printf_print_value:
beq $t2, 'd', printf_print_int
beq $t2, 's', printf_print_str
beq $t2, 'c', printf_print_char
beq $t2, 'x', printf_print_hex

printf_print_int:
move $a0, $t4
li $v0, 1
syscall
addu $s0, $s0, $t7
j printf_arg_done

printf_print_str:
li $t6, 0x00400000
blt $t4, $t6, printf_arg_done
move $a0, $t4
li $v0, 4
syscall
addu $s0, $s0, $t7
j printf_arg_done

printf_print_char:
move $a0, $t4
li $v0, 11
syscall
addu $s0, $s0, $t7
j printf_arg_done

printf_print_hex:
move $t6, $t4
move $t8, $t7

addiu $t9, $t8, -1
sll $t9, $t9, 2

printf_print_hex_loop:
srlv $a0, $t6, $t9
andi $a0, $a0, 0xF

slti $t3, $a0, 10
bnez $t3, printf_hex_digit
addiu $a0, $a0, 87
j printf_hex_output

printf_hex_digit:
addiu $a0, $a0, 48

printf_hex_output:
li $v0, 11
syscall

addiu $t9, $t9, -4
addiu $t8, $t8, -1
bgtz $t8, printf_print_hex_loop

addu $s0, $s0, $t7
j printf_arg_done

printf_arg_done:
addiu $t1, $t1, 4
printf_next:
addiu $t0, $t0, 1
j printf_loop

printf_end:
move $v0, $s0
lw $s0, 24($sp)
lw $ra, 28($sp)
addiu $sp, $sp, 32
jr $ra
"""


def get_scanf_asm():
    return r"""
.text
scanf:
li $t0, 0x00400000
bge $a0, $t0, scanf_proceed
li $v0, 0
jr $ra

scanf_proceed:
addiu $sp, $sp, -32
sw $ra, 28($sp)
sw $a0, 32($sp)
sw $a1, 36($sp)
sw $a2, 40($sp)
sw $a3, 44($sp)

move $t0, $a0
addiu $t1, $sp, 36
li $t3, 0

scanf_loop:
lb $t2, ($t0)
beq $t2, $zero, scanf_end
beq $t2, '%', scanf_format_start
j scanf_next

scanf_format_start:
addiu $t0, $t0, 1
lb $t2, ($t0)
li $t8, 0

scanf_parse_width:
li $t6, 48
blt $t2, $t6, scanf_check_type
li $t6, 57
bgt $t2, $t6, scanf_check_type

mul $t8, $t8, 10
subu $t6, $t2, 48
addu $t8, $t8, $t6

addiu $t0, $t0, 1
lb $t2, ($t0)
j scanf_parse_width

scanf_check_type:
beq $t2, 'd', scanf_int
beq $t2, 'x', scanf_hex
beq $t2, 's', scanf_str
beq $t2, 'c', scanf_char
beq $t2, 'f', scanf_float
j scanf_next

scanf_int:
lw $t4, ($t1)
li $t5, 0x00400000
blt $t4, $t5, scanf_int_skip

li $t5, 0
li $t7, 0
li $t9, 0

scanf_int_skip_whitespace:
li $v0, 12
syscall
li $t6, 32
beq $v0, $t6, scanf_int_skip_whitespace
li $t6, 10
beq $v0, $t6, scanf_int_skip_whitespace
li $t6, 9
beq $v0, $t6, scanf_int_skip_whitespace
li $t6, 13
beq $v0, $t6, scanf_int_skip_whitespace

li $t6, 45
bne $v0, $t6, scanf_int_check_plus
li $t7, 1
j scanf_int_loop_chars

scanf_int_check_plus:
li $t6, 43
bne $v0, $t6, scanf_int_process_char
j scanf_int_loop_chars

scanf_int_loop_chars:
li $v0, 12
syscall

scanf_int_process_char:
li $t6, 48
blt $v0, $t6, scanf_int_done
li $t6, 57
bgt $v0, $t6, scanf_int_done

li $t9, 1
mul $t5, $t5, 10
subu $t6, $v0, 48
addu $t5, $t5, $t6
j scanf_int_loop_chars

scanf_int_done:
beqz $t9, scanf_int_skip
beqz $t7, scanf_int_store
negu $t5, $t5

scanf_int_store:
sw $t5, 0($t4)
addiu $t3, $t3, 1

scanf_int_skip:
addiu $t1, $t1, 4
j scanf_next

scanf_hex:
lw $t4, ($t1)
li $t5, 0x00400000
blt $t4, $t5, scanf_hex_skip_all

addiu $sp, $sp, -32
move $a0, $sp
li $a1, 32
li $v0, 8
syscall

move $t4, $sp
li $v0, 0
li $t7, 0

lb $t5, ($t4)
li $t6, 45
bne $t5, $t6, scanf_hex_loop
li $t7, 1
addiu $t4, $t4, 1

scanf_hex_loop:
lb $t5, ($t4)
beq $t5, $zero, scanf_hex_done
li $t6, 10
beq $t5, $t6, scanf_hex_done
li $t6, 32
beq $t5, $t6, scanf_hex_done

li $t6, 48
blt $t5, $t6, scanf_hex_next
li $t6, 57
bgt $t5, $t6, scanf_hex_alpha

subu $t6, $t5, 48
sll $v0, $v0, 4
addu $v0, $v0, $t6
j scanf_hex_next

scanf_hex_alpha:
li $t6, 97
blt $t5, $t6, scanf_hex_upper
li $t6, 102
bgt $t5, $t6, scanf_hex_next
subu $t6, $t5, 87
sll $v0, $v0, 4
addu $v0, $v0, $t6
j scanf_hex_next

scanf_hex_upper:
li $t6, 65
blt $t5, $t6, scanf_hex_next
li $t6, 70
bgt $t5, $t6, scanf_hex_next
subu $t6, $t5, 55
sll $v0, $v0, 4
addu $v0, $v0, $t6

scanf_hex_next:
addiu $t4, $t4, 1
j scanf_hex_loop

scanf_hex_done:
beqz $t7, scanf_hex_store
negu $v0, $v0

scanf_hex_store:
addiu $sp, $sp, 32
lw $t4, ($t1)
sw $v0, 0($t4)
addiu $t3, $t3, 1

scanf_hex_skip_all:
addiu $t1, $t1, 4
j scanf_next

scanf_float:
lw $t4, ($t1)
li $t5, 0x00400000
blt $t4, $t5, scanf_float_skip

li $v0, 6
syscall
s.s $f0, 0($t4)
addiu $t3, $t3, 1

scanf_float_skip:
addiu $t1, $t1, 4
j scanf_next

scanf_str:
lw $t4, ($t1)
li $t5, 0x00400000
blt $t4, $t5, scanf_str_skip

move $t6, $t4
li $t7, 0

scanf_str_loop:
beqz $t8, scanf_str_read
bge $t7, $t8, scanf_str_done

scanf_str_read:
li $v0, 12
syscall

li $t5, 32
beq $v0, $t5, scanf_str_done
li $t5, 10
beq $v0, $t5, scanf_str_done
li $t5, 9
beq $v0, $t5, scanf_str_done
li $t5, 13
beq $v0, $t5, scanf_str_done
beq $v0, $zero, scanf_str_done

sb $v0, ($t6)
addiu $t6, $t6, 1
addiu $t7, $t7, 1
j scanf_str_loop

scanf_str_done:
sb $zero, ($t6)
addiu $t3, $t3, 1

scanf_str_skip:
addiu $t1, $t1, 4
j scanf_next

scanf_char:
lw $t4, ($t1)
li $t5, 0x00400000
blt $t4, $t5, scanf_char_skip

li $v0, 12
syscall
sb $v0, 0($t4)
addiu $t3, $t3, 1

scanf_char_skip:
addiu $t1, $t1, 4
j scanf_next

scanf_next:
addiu $t0, $t0, 1
j scanf_loop

scanf_end:
move $v0, $t3
lw $ra, 28($sp)
addiu $sp, $sp, 32
jr $ra
"""
