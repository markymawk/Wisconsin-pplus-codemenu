#################################################
Enable ASDI Downwards while Grounded v1.1 [Magus]
#################################################
# Code menu variant by Eon, mawwwk
# Remove matching code in Source/ProjectM/DirectionalInfluence.asm

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02E0

# Original code PM:
# op b 0x4C @ $80876F88
# op b 0x4C @ $80876698
# Original Brawl instr:
# bne- 0x4C @ [same addresses]

HOOK @ $80876F88
{
  bne ASDIdown               # vBrawl compare
  %cmHeader(r3, CodeMenuLoc)
  cmpwi r3, 1                # Skip if toggle ON (PM compare)
  bne %end%

ASDIDown:
  %lwi(r12, 0x80876FD4)		# addr + 0x4C
  mtctr r12
  bctr
}

HOOK @ $80876698
{
  bne ASDIDown_2              # vBrawl compare
  %cmHeader(r3, CodeMenuLoc)
  cmpwi r3, 1               # Skip if toggle ON (PM compare)
  bne %end%

ASDIDown_2:
  %lwi(r12, 0x808766E4)		# addr + 0x4C
  mtctr r12
  bctr
}
