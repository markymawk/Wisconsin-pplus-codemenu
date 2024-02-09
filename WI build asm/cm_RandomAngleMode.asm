########################################
Random Angle Mode 1.2 [PyotrLuzhin, Eon]
########################################
# Code menu variant by Desi, mawwwk

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02BC

HOOK @ $80767ADC
{
  lwz r0, 0x14(r3)				# Get move angle
  stwu r1, -0x10(r1)			# Store registers
  stw r3, 0x08(r1)
  stw r4, 0x0C(r1)
  %cmHeader(r4, CodeMenuLoc)	# Check code menu
  cmpwi r4, 0
  beq end

  li r3, 360
  %randi()
  addi r0, r3, 1		# Store new angle

end:
  lwz r3, 0x08(r1)		# Restore registers
  lwz r4, 0x0C(r1)
  addi r1, r1, 0x10
}