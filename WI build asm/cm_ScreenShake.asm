#########################################
Disable screen shake toggle [Eon, mawwwk]
#########################################
.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = 0x02E4 # Modify for header

HOOK @ $8009D564
{
	mr r0, r3					# Addr should be in r0 already,
	lis r3, CodeMenuStart		# but store it to be safe
	ori r3, r3, CodeMenuHeader
	lwz r3, 0(r3)
	lbz r3, 0xB(r3)
	cmpwi r3, 0					# If toggle off, disable shake
	mr r3, r0					# Restore r3 value
	beq noShake
	cmpwi r4, 3					# Original op
	b %END%

noShake:
	blr							# Ignore cmQuake call
}
