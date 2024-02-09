######################################
Tiebreaker (WI LGL ver.) [Eon, mawwwk]
######################################
.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02CC

HOOK @ $8095617c 
{
	#original command
	stw r0, 0x27C(r3)	# Store stock count

	#if staling enabled = if singleplayer mode, dont enable
	lis r7, 0x80b8
	lwz r7, 0x7C28(r7)
	lwz r7, 0x6E(r7)
	andi. r7, r7, 0x8
	beq %end%
	#if items enabled = casuals, dont enable
	lis r7, 0x805A
	lwz r7, 0xE0(r7)
	lwz r7, 0x08(r7)
	lbz r7, 0x16(r7)
	cmpwi r7, 0
	bne %end%
	
    sth r0, 0x27C(r3) # Store stock count in top half of victory condition word
    lhz r0, 0x2C(r3)  # Load percent for current port
    li r7, 1000
    sub r0, r7, r0
    sth r0, 0x27E(r3) # Store (1000 - percent) into bottom half
	
	# Start ledgegrab chacks
	lwz r7, 0xE0(r31)	# Check frames remaining in game
	cmpwi r7, 0			# Only apply as a wincon for timeout
	bne %end%
	
	# TO USE WITHOUT CODE MENU TOGGLE, 
	# remove the code menu check below,
	# and uncomment this line:
	# li r7, 45									
	
	%cmHeader(r7, CodeMenuLoc)	# Code menu check
	cmpwi r7, 0                 # Skip if toggle OFF
	beq %end%
	mulli r7, r7, 15			# LGL = (codeMenuVal * 15) + 15
	addi r7, r7, 15				#
	
checkLedgegrabs:
	lwz r0, 0x228(r3)		# Get ledgegrab count for current port
	cmpw r0, r7				# \ If not over the limit, treat as 0
	bge checkByteOverflow	# |
	li r0, 0				# |
	b storeLedgegrabs		# /

checkByteOverflow:
	cmpwi r0, 255			# Only have room for one byte, so max at 255
	ble storeLedgegrabs
	li r0, 255

storeLedgegrabs:
	li r7, 255				# \ Store (255 - grabs) to invert the count
	sub r0, r7, r0			
	stb r0, 0x27C(r3)		# / Fewer grabs = higher val = prioritize winner
	
}