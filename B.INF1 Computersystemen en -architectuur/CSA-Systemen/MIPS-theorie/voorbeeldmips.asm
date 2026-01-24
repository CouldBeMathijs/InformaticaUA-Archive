#	programmingConstructs.asm
#
#   #define MAXi 6
#
#	unsigned int i = 0;
#	unsigned int res = 1;
#
#	do
#		if (i is odd) then res = 2*res + 1;
#		i++;
#	while (i < 6);
		
		.eqv	MAXi, 6
	
		.data
i:		.word	0   				# the variable i, initialized
res:	.word	1   				# the variable res, initialized
		.text

		la		$s0, i
		lw		$s0, 0($s0)			# load the value of i in $s0
		lw		$s1, res			# load the value of res in $s1
		                            # assembler translates to
		                            #  lui base address of .data, into assembler temporary $ta
		                            #  lw from address in $ta with appropriate offset, into $s1
	
DoWhile:andi	$t0, $s0, 1			# "mask" to select only bit 0
		beq		$t0, $zero, end_if	# if this bit is 0, i is even (NOT odd)
		sll		$s1, $s1, 1			# 2*res
		addiu	$s1, $s1, 1			# +1
end_if:	addiu	$s0, $s0, 1			# i++
		sltiu	$t0, $s0, MAXi		# is i < MAXi
		bne		$t0, $zero, DoWhile	# if result of sltiu is 1 (NOT zero), go back to DoWhile
		
		la		$t0, i
		sw		$s0, 0($t0)			# store value of i in memory
		la		$t0, res
		sw		$s1, 0($t0)			# store value of res in memory
					
	
	
