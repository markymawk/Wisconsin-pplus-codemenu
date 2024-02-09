########################
Grab trade effects [Eon]
########################
# Code menu variant by mawwwk
# 0 = default. 1 = grabs recoil (trade). 2 = Manaphy Heart Swap effect

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02EC

HOOK @ $8077055c
{
  %cmHeader(r31, CodeMenuLoc)
  cmpwi r31, 0                  # If not set to default behavior,
  bne grabsClank				# use clank behavior
  
  %call(0x807977C8)				# original op: bl 0x807977c8
  b %END%

grabsClank:
  mr r31, r3 		#captured main
  %call(0x807977B8)	#getCatchModule

  lfs f1, 0x54(r3)				# \ if distances between grabs different then just jump to end,
  lfs f2, 0x54(r30)				#   they didnt grab each other,
  fcmpo cr0, f1, f2 			# / this is done since target of grab never gets cleared, but distance is reset every frame
  bne end
  lwz r4, 0x30(r3) 				# gets who the opponent has grabbed
  
  lwz r3, 0x10(r30) 			# compare to self id
  lwz r3, 0x8(r3)
  lwz r3, 0x28(r3)
  cmpw r3, r4 					# if self id and target's target id match then they grabbed you
  bne end
  
  lwz r3, 0x10(r30)
  lwz r3, 0xD8(r3)
  lwz r3, 0x70(r3)
  lwz r12, 0x0(r3)
  lwz r12, 0x48(r12)
  mtctr r12
  bctrl 
  cmpwi r3, 0x34 #grab 
  beq recoil
  cmpwi r3, 0x36 #dashgrab 
  beq recoil
  cmpwi r3, 0x38 #pivotgrab
  bne end
  
recoil:
  %cmHeader(r3, CodeMenuLoc)
  cmpwi r3, 2                	# Check for Heart Swap toggle
  beq heartSwap

  lwz r5, 0x10(r30)
  lwz r3, 0xD8(r5)
  lwz r3, 0x70(r3)
  li r4, 0x3B
  lwz r12, 0x0(r3)
  lwz r12, 0x14(r12)
  mtctr r12
  bctrl 
  #set RA-Basic[7] to 1 when grab-trade occurs
  lwz r3, 0x10(r30)
  lwz r3, 0xD8(r3)
  lwz r3, 0x64(r3)
  li r4, 1

  lis r5, 0x1000
  ori r5, r5, 0x0007
  lwz r12, 0x0(r3)
  lwz r12, 0x1C(r12)
  mtctr r12
  bctrl
  b recoilEnd
  
heartSwap:
  mr r3, r31
  lis r4, 0x1000
  %call(0x807976D8)
  mr r31, r3
  #set RA-Basic[7] to 1 when grab-trade occurs
  lwz r3, 0x10(r30)
  lwz r3, 0xD8(r3)
  lwz r3, 0x64(r3)
  lis r4, 0x1000
  lwz r12, 0x0(r3)
  lwz r12, 0x18(r12)
  mtctr r12
  bctrl 
  mr r5, r3
  lis r3, 0x80B8
  lwz r3, 0x7C28(r3)
  lwz r3, 0x154(r3)
  li r4, 1
  mr r6, r31
  li r7, 1
  li r8, 540		# Timer for swap (in frames?)
  %call(0x80824090)

recoilEnd:
  %lwi(r12, 0x807705B4)
  mtctr r12
  bctr
  
end:
  mr r3, r31
  %call(0x807977C8)
}