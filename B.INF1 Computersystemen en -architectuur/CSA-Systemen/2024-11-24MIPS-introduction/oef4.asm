.data
# Geheugenplaats reserveren voor alle adressen van de cases
jump_table:
    .word case_0
    .word case_1
    .word case_2
    .word default_case

.text
main:
    # Lees de waarde voor n in op $t0
    li $v0, 5
    syscall
    move $t0, $v0

    # Grootte van de jump table (3 cases)
    li $t1, 3
    # Als we naar een van de cases kunnen gaan (omdat n < 3)
    blt $t0, $t1, jump
    la $t1, default_case
    jr $t1

jump:
    # Bereken het adres voor de juiste case
    # Laad de basis van de jump table
    la $t1, jump_table
    # Vermenigvuldig n met 4 (woordgrootte)
    sll $t0, $t0, 2
    # Voeg offset toe aan basisadres
    add $t1, $t1, $t0   
    # Haal het doeladres op uit de tabel
    lw $t1, 0($t1)
    # En spring er naartoe
    jr $t1              
    

case_0:
    # a = 9
    li $t2, 9
    b exit_case         # Ga naar exit_case

case_1:
    # a = 6
    li $t2, 6
    # Geen break, dus doe gewoon verder

case_2:
    # a = 8
    li $t2, 8
    b exit_case         # Ga naar exit_case

default_case:
    # a = 7
    li $t2, 7

exit_case:
    # Druk de waarde van a af ($t2)
    li $v0, 1
    move $a0, $t2
    syscall
exit:
    li $v0, 10
    syscall
