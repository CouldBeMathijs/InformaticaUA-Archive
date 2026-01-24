.globl main

.data
filename: .asciiz "/home/mathijs/Documents/B.INF1/CSA/Systemen/MIPSPj1/files/test_file_2.txt"
bestandslocatie: .space 4096    # Buffer to store file contents
.text

main:
    # Open the file (syscall 13)
    li $v0, 13                 # Syscall: open file
    la $a0, filename          # File name
    li $a1, 0                  # Mode: read-only
    li $a2, 0                  # Unused for read-only
    syscall

    move $a0, $v0              # File descriptor returned in $v0

    # Read the file (syscall 14)
    li $v0, 14                 # Syscall: read file
    la $a1, bestandslocatie    # Buffer to store data
    li $a2, 4096               # Maximum number of bytes to read
    syscall

    # Print the contents of the buffer
    li $v0, 4                  # Syscall: print string
    la $a0, bestandslocatie    # Address of the buffer
    syscall

exit:
    # Exit the program
    li $v0, 10                 # Syscall: exit
    syscall
