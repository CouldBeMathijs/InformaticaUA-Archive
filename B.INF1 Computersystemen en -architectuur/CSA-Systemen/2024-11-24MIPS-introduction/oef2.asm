.data
new_line: .asciiz "\n"
.text
main:
	# Lees een integer n in van de gebruiker
	li $v0, 5
	syscall
	move $s0, $v0
	# Laat i = 1
	li $t0, 1
for:	
	# i<= n
	blt $s0, $t0, exit
	j inside_for
inside_for:
	#print i
	move $a0, $t0
	li $v0, 1
	syscall
	#print de nieuwe lijn
	la $a0, new_line
	li $v0, 4
	syscall
	# voeg 1 to bij i
	addi $t0, $t0, 1
	j for
exit:
	li $v0, 10          
	syscall
