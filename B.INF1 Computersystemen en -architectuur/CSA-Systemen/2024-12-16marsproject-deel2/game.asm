 .globl main

.data
# Voor te uploaden
# mazeFilename:	 .asciiz "input_1.txt"
# Voor te testen
mazeFilename:    .asciiz "/home/mathijs/Documents/B.INF1/CSA-Systemen/2024-12-16marsproject-deel2/input_1.txt"
buffer:          .space 4096
victoryMessage:  .asciiz "\nYou have won the game!\n"
endaddress:	 .word 0x100087FC # The last pixel of the screen
fileError:	 .asciiz "The file was not able to be read, exiting now..."
Instructions:	 .asciiz "You can move using WASD, press X to exit, get to the exit to win!\n"

amountOfRows:    .word 16  # The mount of rows of pixels
amountOfColumns: .word 32  # The mount of columns of pixels

wallColor:      .word 0x004286F4    # Color used for walls (blue)
passageColor:   .word 0x00000000    # Color used for passages (black)
playerColor:    .word 0x00FFFF00    # Color used for player (yellow)
exitColor:      .word 0x0000FF00    # Color used for exit (green)

.text

main:
read_file_to_buffer:
    # Open the file (syscall 13)
    li $v0, 13                 # Syscall: open file
    la $a0, mazeFilename       # File name
    li $a1, 0                  # Mode: read-only
    li $a2, 0                  # Unused for read-only
    syscall
    bltz $v0, exitFileError
    move $a0, $v0              # File descriptor returned in $v0, needed for syscall 14

    # Read the file (syscall 14)
    li $v0, 14                 # Syscall: read file
    la $a1, buffer    	       # Buffer to store data
    li $a2, 4096               # Maximum number of bytes to read
    syscall
    
    # Close the file again (syscall 16)
    li $v0, 16
    syscall
    
    #Print the instructions to the screen
    la $a0, Instructions
    li $v0, 4
    syscall
draw_screen:
    # load the maze to the screen
    # Laadt de kleuren in de registers $t1-$t4
    lw $t1, wallColor
    lw $t2, passageColor
    lw $t3, playerColor
    lw $t4, exitColor
    # Laadt het startadres in van de buffer in $s0
    la $s0, buffer
    move $t0, $gp #Begin te tekenen vanaf de global pointer
    addi $s0, $s0,-1
    addi $t0, $t0,-4
draw_while:
    addiu $t0,$t0, 4
    addiu $s0,$s0, 1
    lb $t5, 0($s0)             # Load the current byte into $t5
    beq $t5, 119, draw_wall 	#if the current char is 'w'
    beq $t5, 112, draw_passage	#if the current char is 'p'
    beq $t5, 10, new_line_ignore
    beq $t5, 115, draw_player	#if the current char is 's'
    beq $t5, 117, draw_exit 	#if the current char is 'u'
    beq $t5, 0, game_loop		#Once we've finished drawing, exit the while loop
    #else, this code should never run
    j exit
draw_wall:
    sw $t1, 0($t0)
    j draw_while
draw_passage:
    sw $t2, 0($t0)
    j draw_while
new_line_ignore:
    subi $t0, $t0 4
    j draw_while
draw_player:
    # Sla de positie van de speler op in $s7
    move $s7, $t0
    sw $t3, 0($t0)
    j draw_while
draw_exit:
    sw $t4, 0($t0)
    j draw_while

game_loop:
    li $v0, 12	
    syscall
    beq $v0, 119, try_up
    beq $v0, 97,  try_left
    beq $v0, 115, try_down
    beq $v0, 100, try_right
    beq $v0, 120, exit
j game_loop
try_up:
    # Calculate the position the player should move to
    addiu $s6, $s7, -128
    move $a1, $s6
    move $a0, $s7
    jal move_player_to
    move $s6, $s7
    j game_loop
try_left:
    addiu $s6, $s7, -4
    move $a1, $s6
    move $a0, $s7
    jal move_player_to
    move $s6, $s7
    j game_loop
try_right:
    addiu $s6, $s7, 4
    move $a1, $s6
    move $a0, $s7
    jal move_player_to
    move $s6, $s7
    j game_loop
try_down:
    addiu $s6, $s7, 128
    move $a1, $s6
    move $a0, $s7
    jal move_player_to
    move $s6, $s7
    j game_loop
    

reverse_translate_coords:
    # Berekent de kolom- en rijnummers van een adres
    # a0, adres
    # v0, rijnummer
    # v1, kolomnummer
    move $v0, $a0        
    sub $v0, $v0, $gp    # Trek gp van het adress af
    andi $v1, $v0, 127   # sla de eerste 5bits op in v1, de kolomnummer gaat maar tot 32
    srl $v0, $v0, 7      # Compenseer voor de lengte van de rij in het geheugen
    srl $v1, $v1, 2      # Compenseer voor de lengte van de kolom in het geheugen
    jr $ra
translate_coordinates:
    # Parameters:
    # $a0 = rijnummer
    # $a1aa = kolomnummer
    # $v0 = pixeladres (returnwaarde)

    # Bereken het pixeladres
    sll $a0, $a0, 7             # Rij * 128 (32 pixels * 4 bytes)
    move $v0, $gp               # Basisadres in $v0
    addu $v0, $v0, $a0          # Voeg rij-offset toe
    sll $a1, $a1, 2             # Kolom * 4
    addu $v0, $v0, $a1          # Voeg kolom-offset toe
    jr $ra                       # Return
move_player_to:
    # a0: huidige locatie van de speler
    # a1: nieuwe locatie van de speler
    # s7: updates the player position in s7 automagically

    # Configure the stack
    addi $sp, $sp, -24         # Allocate 16 bytes on the stack
    sw $ra, 12($sp)            # Save the return address on the stack
    sw $t0, 8($sp)             # Save $t0 on the stack
    sw $t1, 4($sp)             # Save $t1 on the stack
    sw $t2, 0($sp)             # Save $t2 on the stack
    sw $t3, 16($sp)	       # Save $t3 on the stack

    # Check if the color at the new position is valid
    lw $t0, 0($a1)             # Load the color at the new position into $t0
    lw $t1, passageColor         # Load the valid color into $t1
    bne $t0, $t1, invalid_move # If the colors do not match, branch to invalid_move
    
    li $v0, 1
    move $s7, $a1
    # Proceed with the move
    lw $t0, passageColor       # Load colorPassage into $t0
    lw $t1, playerColor        # Load colorPlayer into $t1
    sw $t0, 0($a0)             # Set passage color at the current player location
    sw $t1, 0($a1)             # Set player color at the new player location

    # Clean up and return
    lw $t2, 0($sp)             # Restore $t2 from the stack
    lw $t1, 4($sp)             # Restore $t1 from the stack
    lw $t0, 8($sp)             # Restore $t0 from the stack
    lw $ra, 12($sp)            # Restore the return address
    lw $t3, 16($sp)	       # Restore $t3 from the stack
    addi $sp, $sp, 24          # Deallocate the stack space
    jr $ra                     # Return to the caller

invalid_move:
    # Handle invalid move (e.g., just return without doing anything)
    lw $t3, exitColor
    beq $t0, $t3, exit_win
    li $v0, 0
    lw $t2, 0($sp)             # Restore $t2 from the stack
    lw $t1, 4($sp)             # Restore $t1 from the stack
    lw $t0, 8($sp)             # Restore $t0 from the stack
    lw $ra, 12($sp)            # Restore the return address
    lw $t3, 16($sp)	       # Restore $t3 from the stack
    addi $sp, $sp, 24          # Deallocate the stack space
    jr $ra                     # Return to the caller

exit_win:
    # syscall to end the program
    li $v0, 4
    la $a0, victoryMessage
    syscall
exit:
    lw $t1, endaddress
    lw $t2, passageColor
    move $t0, $gp
    li $v0, 32
    li $a0, 4

while:
        syscall
 	bgt $t0, $t1, exit_syscall
 	# Kleur in
 	sw $t2, 0($t0)
 	# Incrementeer
 	addiu $t0,$t0 4
 	j while
exit_syscall:
    li $v0, 10
    syscall
exitFileError:
    li $v0, 4
    la $a0, fileError
    syscall
    li $v0, 17
    li $a0, 1
    syscall
    
    
