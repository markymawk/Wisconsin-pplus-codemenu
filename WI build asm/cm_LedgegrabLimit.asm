###################################
Tiebreaker (WI LGL ver.) [Eon, mawwwk]
###################################

.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = 0x02F8

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
	
	# START LEDGEGRAB CHECKS
	lwz r7, 0xE0(r31)			# Check frames remaining in game
	cmpwi r7, 0					# Only apply ledgegrabs as a wincon for timeout
	bne %end%
	
	lis r7, CodeMenuStart		# \ Load LGL toggle value
	ori r7, r7, CodeMenuHeader  
	lwz r7, 0(r7)
	lbz r7, 0xB(r7)
	cmpwi r7, 0                 # / Skip if toggle OFF
	beq %end%
	
	addi r7, r7, 5				# LGL = (menuVal + 5) * 5
	mulli r7, r7, 5
	
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
	li r7, 255
	sub r0, r7, r0			# \ Store (255 - grabs) to invert the count
	stb r0, 0x27C(r3)		# /
	
}