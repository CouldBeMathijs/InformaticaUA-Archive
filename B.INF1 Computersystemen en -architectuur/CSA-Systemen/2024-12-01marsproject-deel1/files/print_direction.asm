 .globl main

.data
z_text: .asciiz "up\n"
s_text: .asciiz "down\n"
q_text: .asciiz "left\n"
d_text: .asciiz "right\n"
else_t: .asciiz "Unknown input! Valid inputs: z s q d x\n"
.text

main:
    loop:
    # Wait two seconds
    li $v0, 32
    li $a0, 2000
    syscall
    # Read the character from the keyboard MMIO
    la $t0, 0xffff0004   # Load address of the keyboard input
    lw $v0, 0($t0)        # Load the character from the keyboard input
    # Check what character it is
    beq $v0, 122, display_z # If character = 'z'
    beq $v0, 115, display_s # If character = 's'
    beq $v0, 113, display_q # If character = 'q'
    beq $v0, 100, display_d # If character = 'd'
    beq $v0, 120, exit	    # If character = 'x'
    # Else
    # Print else string
    la $a0, else_t
    li $v0, 4 
    syscall
    j loop
display_z:
    la $a0, z_text
    li $v0, 4 
    syscall
    j loop
display_s:
    la $a0, s_text
    li $v0, 4 
    syscall
    j loop
display_q:
    la $a0, q_text
    li $v0, 4 
    syscall
    j loop
display_d:
    la $a0, d_text
    li $v0, 4 
    syscall
    j loop
exit:
    # syscall to end the program
    li $v0, 10    
    syscall
