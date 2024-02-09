########################
Big Head Mode v1.0 [Eon]
########################
# Code menu variant by Desi
# WI: changed to 2 size options from 4

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02B8

HOOK @ $80839010
{ 
  stwu r1, -0x20(r1)
  mflr r0
  stw r0, 0x24(r1)
 
  %cmHeader(r3, CodeMenuLoc)
  cmpwi r3, 1
  blt return

  #getCameraSubject(0)
  lwz r3, 0xD8(r31)
  lwz r3, 0x60(r3)
  lwz r3, 0x18(r3)
  li r4, 0
  lwz r12, 0x0(r3)
  lwz r12, 0xC(r12)
  mtctr r12
  bctrl 
 
  lwz r4, 0x10(r3)

  %cmHeader(r3, CodeMenuLoc)
  lis r0, 0x4000			# Load 1x big head size
  cmpwi r3, 2				# If 2, set 2x big head size
  bne Size_Set
  lis r0, 0x4050

Size_Set:

  lwz r4, 0x0(r4) #HeadN node ID 
  stw r0, 0x8(r1)
  stw r0, 0xC(r1)
  stw r0, 0x10(r1)
  addi r5, r1, 0x8
 
  lwz r3, 0xD8(r31)
  lwz r3, 0x4(r3)
  lwz r12, 0x8(r3)
  lwz r12, 0x68(r12)
  mtctr r12 
  bctrl 
 
 return:
  lwz r0, 0x24(r1)
  mflr r0
  addi r1, r1, 0x20
 
orig:
  lis r3, 0x80B8
}