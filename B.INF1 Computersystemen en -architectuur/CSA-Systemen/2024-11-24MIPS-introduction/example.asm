	.data
begin_message:		.asciiz "Message: Hello, "
empty_space_for_string:	.space 20
line_end:		.asciiz "\n"
	.text
main:
	# read name
	la $a0, empty_space_for_string
	li $a1, 20
	li $v0, 8
	syscall

	# print hello
	la	$a0, begin_message
	li 	$v0, 4
	syscall
		
	# print name
	la $a0, empty_space_for_string
	li $v0, 4
	syscall

	# print newline
	la	$a0, line_end
	li 	$v0, 4
	syscall	
exit:
	li	$v0, 10		# load code for exit
	syscall
