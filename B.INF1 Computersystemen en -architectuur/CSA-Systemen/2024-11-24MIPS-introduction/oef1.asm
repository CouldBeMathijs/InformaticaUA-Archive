.data
print_string: .asciiz "This is my "
print_string2: .asciiz "-th MIPS-program\n"
.text
main:
	# Read an integer from the user into $s0
	li $v0, 5          
	syscall
	move $s0, $v0      

	# Print the first part of the string
	la $a0, print_string 
	li $v0, 4            
	syscall

	# Print the number entered by the user
	move $a0, $s0       
	li $v0, 1           
	syscall

	# Print the second part of the string
	la $a0, print_string2
	li $v0, 4             
	syscall

exit:
	li $v0, 10          
	syscall
