.data
amountOfRows:    .word 16          # 16 rijen
amountOfColumns: .word 32          # 32 kolommen
colorRed:        .word 0x00FF0000  # Rode kleur
colorYellow:     .word 0x00FFFF00  # Gele kleur
endaddress:	 .word 0x10008778  # Tot waar rood gekleurd mag worden

.text
main:
# Initialise to make entire screen red
    lw $t1, endaddress
    lw $t2, colorRed
    # Start coloring from the second position on the second row
    move $t0, $gp
    addiu $t0, $t0 132
# While loop to color the center red
while:
 	bgt $t0, $t1, end_of_while
 	# Kleur in
 	sw $t2, 0($t0)
 	# Incrementeer
 	addiu $t0,$t0 4
 	j while
end_of_while:
# Initialiseer de waarden om de randen in te beginnen kleuren
    #t4 is rijnummer
    li $t4, 0
    #t5 is kolomnummer
    li $t5, 0
    #de kleur is geel
    la $a2, colorYellow
top_row_lus:
# Plaats de argumenten op de juiste plaats
    move $a0, $t4
    move $a1, $t5
    # Roep de functie op
    jal color_pixel
    # incrementeer
    addiu $t5, $t5 1
    # als we op het einde zijn, stop
    bge $t5, 32, pre_left_row_lus
    j top_row_lus
# Onderstaande lussen zijn analoog
pre_left_row_lus:
    li $t4, 0
    li $t5, 0
left_row_lus:
    move $a0, $t4
    move $a1, $t5
    jal color_pixel
    addiu $t4, $t4, 1
    bge $t4, 16, pre_bottom_row
    j left_row_lus
pre_bottom_row:
    li $t4, 15
    li $t5, 0
bottom_row_lus:
    move $a0, $t4
    move $a1, $t5
    jal color_pixel
    addiu $t5, $t5, 1
    bge $t5, 31, pre_right_row
    j bottom_row_lus
pre_right_row:
    li $t4, 0
    li $t5, 31
right_row_lus:
    move $a0, $t4
    move $a1, $t5
    jal color_pixel
    addiu $t4, $t4, 1
    bge $t4, 16, exit
    j right_row_lus
exit:
	li $v0, 10
	syscall

color_pixel:
    # Parameters:
    # $a0 = rijnummer
    # $a1 = kolomnummer
    # $a2 = adres van kleur
    addi $sp, $sp, -4           # Reserveer stackruimte voor $ra
    sw $ra, 0($sp)              # Sla returnadres op

    # Bereken adres van pixel
    jal translate_coordinates     # Roept translate_coordinates aan
    lw $t1, 0($a2)               # Laad kleur uit $a2
    sw $t1, 0($v0)               # Sla kleur op in pixeladres

    lw $ra, 0($sp)               # Herstel returnadres
    addi $sp, $sp, 4             # Maak stackruimte vrij
    jr $ra                       # Return

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