#########################################
Disable screen shake toggle [Eon, mawwwk]
#########################################

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02D0

HOOK @ $8009D564
{
	mr r0, r3				# Addr should be in r0 already but to be safe
	%cmHeader(r3, CodeMenuLoc)
	cmpwi r3, 0				# If toggle off, disable shake
	mr r3, r0				# Restore r3 value
	bne cameraShake
	blr						# Ignore cmQuake call

cameraShake:
	cmpwi r4, 3				# Original op						
}
