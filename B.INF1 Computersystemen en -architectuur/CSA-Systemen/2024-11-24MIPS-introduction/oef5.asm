.data
string_prime: .asciiz "--Prime--\n"
string_not_prime: .asciiz "--No prime--\n"

.text
main:
	# Lees het getal n in $s0
	li $v0, 5
	syscall
	move $s0, $v0

	# Getallen kleiner dan 2 zijn nooit priemgetallen
	li $t1, 1
	ble $s0, $t1, not_prime

	# Initialiseer een teller (teller = 2)
	li $t0, 2

for_lus:
	# Controleer of teller >= n
	beq $t0, $s0, is_prime

	# Bereken n % teller door herhaaldelijk aftrekken
	move $t3, $s0      # $t3 = kopie van n
modulus_loop:
	sub $t3, $t3, $t0  # $t3 -= $t0
	bltz $t3, after_modulus_check # als $t3 < 0, stop aftrekken

	j modulus_loop

after_modulus_check:
	# Controleer of rest (t3) == 0
	add $t3, $t3, $t0  # corrigeer, $t3 = $t3 + $t0
	beq $t3, $zero, not_prime # als $t3 (rest bij deling) == 0, geen priemgetal

	# teller++
	addi $t0, $t0, 1

	j for_lus

is_prime:
	# Toon "--Prime--"
	la $a0, string_prime
	li $v0, 4
	syscall
	j exit

not_prime:
	# Toon "--No prime--"
	la $a0, string_not_prime
	li $v0, 4
	syscall

exit:
	li $v0, 10
	syscall
