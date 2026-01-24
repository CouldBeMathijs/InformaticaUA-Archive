.data
spatie: .asciiz " "
new_line: .asciiz "\n"

.text
main:
    # Lees een integer n in van de gebruiker
    li $v0, 5          # Syscall om integer te lezen
    syscall
    move $s0, $v0      # Sla de waarde van n op in $s0

    # Initialiseer i en j
    li $t0, 1          # $t0 = i (voor de buitenste lus)

for_lijn:
    # Controleer of i > n (buitenste lus)
    bgt $t0, $s0, exit # Stop als $t0 > $s0

    # Initialiseer j voor de binnenste lus
    li $t1, 1          # $t1 = j (voor de binnenste lus)

for_getal:
    # Controleer of j > i (binnenste lus)
    bgt $t1, $t0, lijn_af

    # Print het huidige getal (j)
    move $a0, $t1      # Laad j in $a0
    li $v0, 1          # Syscall om integer te printen
    syscall

    # Print een spatie
    la $a0, spatie     # Laad spatie in $a0
    li $v0, 4          # Syscall om string te printen
    syscall

    # Vergroot j met 1
    addi $t1, $t1, 1   # j++

    # Herhaal de binnenste lus
    j for_getal

lijn_af:
    # Print een nieuwe lijn
    la $a0, new_line   # Laad newline in $a0
    li $v0, 4          # Syscall om string te printen
    syscall

    # Vergroot i met 1
    addi $t0, $t0, 1   # i++

    # Herhaal de buitenste lus
    j for_lijn

exit:
    # Exit programma
    li $v0, 10         # Syscall om programma te beëindigen
    syscall
