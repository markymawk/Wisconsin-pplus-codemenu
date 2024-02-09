###################################
[WI Netplay] Treat stage choice setting as ordered [mawwwk]
###################################

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E030C

HOOK @ $80055428
{
	%cmHeader(r12, CodeMenuLoc)
	cmpwi r12, 0; beq normal	# If off, run original op
	
	li r0, 3		# If on, force ordered setting
	b %END%
normal:
	lbz r0, 7(r3)	# Original op
}