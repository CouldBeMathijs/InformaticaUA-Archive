.data
pi: .float 3.14
.text
main:
	# Lees een float in $f2
	li $v0, 6
	syscall
	mov.s $f2, $f0
	# Bereken r^2 en sla het op in $f2
	mul.s $f2, $f2, $f2
	# Laadt pi in $f0
	l.s $f0, pi
	# Bereken r^2*pi en sla het op in $f12
	mul.s $f12, $f0, $f2
	# Print de uitkomst naar het scherm
	li $v0, 2
	syscall
exit:
	li $v0, 10
	syscall
