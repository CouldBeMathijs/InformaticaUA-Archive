.globl main

.data

amountOfRows:    .word 16  # The mount of rows of pixels
amountOfColumns: .word 32  # The mount of columns of pixels

promptRows: .asciiz "Please enter the row number:\n"
promptCols: .asciiz "Please enter the column number:\n"

msgShowMemoryAddress: .asciiz "The memory address for the pixel is:\n"

.text

main:
    	#Start of function
    li $v0, 4		# print string
    la $a0, promptRows  # message to ask the user for the row number
    syscall
    li $v0, 5  # read integer
    syscall    # ask the user for a row number
    move $t0, $v0
    
    li $v0, 4		# print string
    la $a0, promptCols  # message to ask the user for the column number
    syscall
    li $v0, 5  # read integer
    syscall    # ask the user for a column number
    move $t1, $v0
    
    
    li $v0, 4		# print string
    la $a0, msgShowMemoryAddress
    syscall
    move $a0, $t0
    move $a1, $t1
    jal translate_coordinates
    move $a0, $v0
    
    li $v0 1
    syscall
    j exit
    	#End of function 
translate_coordinates:
    # Parameters:
    # $a0 = rijnummer
    # $a1 = kolomnummer
    # $v0 = pixeladres (returnwaarde)

    # Bereken het pixeladres
    sll $a0, $a0, 7             # Rij * 128 (32 pixels * 4 bytes)
    move $v0, $gp               # Basisadres in $v0
    addu $v0, $v0, $a0          # Voeg rij-offset toe
    sll $a1, $a1, 2             # Kolom * 4
    addu $v0, $v0, $a1          # Voeg kolom-offset toe
    jr $ra                       # Return

exit:

    li $v0, 10  # syscall to end the program
    syscall
