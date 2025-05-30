######################################################################################
L-Cancel Landing Lag and Success Rate and Score Display is Auto L-Cancel Option + White L-cancel Flash v3.0 [Magus, Standardtoaster, wiiztec, Eon]
######################################################################################
# Code menu variant by Desi, based on per-player versions by wiiztec
# Code adapted to WI code menu by mawwwk
# Removed dependency on per-port ALC, ALC efficacy, and input buffer toggles, and added working flash for global ALC miss
# Requires REMOVAL of similarly-named code inside Source/Project+/L-Cancel.asm

.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = 0x02A8       #Offset of word containing location of the player 1 toggle. Source is compiled with headers for this.

HOOK @ $80874850 
{
	cmpwi r3, 0x5
	mr r31, r3
	bne end
	#soColorBlendModule
	lwz r3, 0xD8(r28)
	lwz r3, 0xAC(r3)
	li r4, 1
	lwz r12, 0(r3)
	lwz r12, 0x20(r12)
	mtctr r12
	bctrl
end:
	lwz r4, 0xD8(r28)
}

# Land and detect L-cancel state, set flash and stat appropriately
op nop @ $8081BE8C
HOOK @ $8087459C
{
loc_0x0:
#get LA-Basic[90]
  lwz r3, 0xD8(r31)
  lwz r3, 0x64(r3)
  lis r4, 0x1000
  ori r4, r4, 90
  
  lwz r12, 0x0(r3)
  lwz r12, 0x18(r12)
  mtctr r12
  bctrl 
  cmpwi r3, 0
  ble missedLCancel

trueLcancel:
  #Set R0 to White, branch to Apply Flash
  lis r0, 0xFFFF			# RGBA (255, 255, 255, 220)
  ori r0, r0, 0xFFDC	
  bl 0x4  					#set LR
  mflr r11 					#Store Link Register in R11
  addi r11, r11, 0xC	
  bl applyFlash	
  li r6, 1					# count as success for stats
  lfs f0, -23448(r2)		# load 0.5
  fmuls f30, f30, f0
  b calcStat

missedLCancel: 
  lis r11, 0x805A		# P+ 2.5 change
  lwz r11, 0xE0(r11)	
  lwz r11, 0x08(r11)		
  lbz r11, 0xE5(r11)	# 0x4D (+ 0x98)
  andi. r11, r11, 1	# bit used for ALC
  bne applyLcancelALC
  lwz r11, 28(r31)          # \ Begin code menu check here.
  lwz r11, 40(r11)          # | Obtain player ID
  lwz r11, 16(r11)          # | 
  lbz r11, 85(r11)          # /
  mulli r11, r11, 0x4       # Determine which player offset to load
  lis r6, CodeMenuStart
  ori r6, r6, CodeMenuHeader    #Load Code Menu Header
  lwzx r6, r6, r11
  lbz r11, 0xB(r6)
  cmpwi r11, 1				# If (CodeMenuVar == 1), apply red flash on miss
  beq applyNoCancelRedFlash
  li r6, 0 					# Else: missed L-Cancel, no flash
  b calcStat				

applyNoCancelRedFlash:
  lis r0, 0xFF00			# \ Red Flash
  ori r0, r0, 0x00D0		# / RGBA (255, 0, 0, 208)
  bl 0x4  					# set LR
  mflr r11 					# Store Link Register in r11
  addi r11, r11, 0xC
  bl applyFlash
  li r6, 0
  b calcStat

applyLcancelALC:
  lfs f0, -23448(r2)		# load 0.5
  fmuls f30, f30, f0
  lwz r11, 28(r31)          # \ Begin code menu check here.
  lwz r11, 40(r11)          # | Obtain player ID
  lwz r11, 16(r11)          # | 
  lbz r11, 85(r11)          # /
  mulli r11, r11, 0x4       # Determine which player offset to load
  lis r6, CodeMenuStart
  ori r6, r6, CodeMenuHeader    #Load Code Menu Header
  lwzx r6, r6, r11
  lbz r11, 0xB(r6)
  cmpwi r11, 1				# If (CodeMenuVar == 1), apply purple flash on miss
  beq applyNoCancelPurpleFlash
  li r6, 0 					# Else, missed L-Cancel, no flash
  b calcStat

applyNoCancelPurpleFlash:
  lis r0, 0x7000			# \ Purple Flash
  ori r0, r0, 0xFFD0		# / RGBA (112, 0, 255, 208)
  bl 0x4  					# set LR
  mflr r11 					# Store Link Register in r11
  addi r11, r11, 0xC
  bl applyFlash
  li r6, 0
  b calcStat
  
#everything past this point is for the stat
calcStat:
#add one to total aerial count
  cmpwi r6, 0x0				# Check for L-cancel to branch later
  lis r6, 0x80B8
  ori r6, r6, 0x8394
  lfs f6, 0(r6)
  #gets a pointer to LA-Basic data
  lwz r4, 0xD8(r31)
  lwz r4, 0x64(r4)
  lwz r4, 0x20(r4)
  lwz r4, 0xC(r4)

  lfs f5, 0x238(r4)
  fadds f5, f5, f6
  stfs f5, 0x238(r4)

  lis r5, 0x80B8
  lwz r5, 0x7C28(r5)
  lwz r5, 0x154(r5)
  lwz r5, 0(r5)
  lwz r6, 0x8(r31)
  lwz r6, 0x10C(r6)
  rlwinm r6, r6, 0, 24, 31
  mulli r6, r6, 0x244
  add r5, r5, r6
  lwz r5, 40(r5)
  addi r5, r5, 0x850
  ble stats_noLcancel	# If r6 == 0 above
  
  lis r6, 0x80B8		# else, successful LC
  ori r6, r6, 0x8394
  lfs f6, 0(r6)
  lfs f4, 572(r4)
  fadds f4, f6, f4
  stfs f4, 572(r4)

stats_noLcancel:
  lfs f4, 572(r4)
  fdivs f5, f4, f5
  lis r6, 0x80B8
  ori r6, r6, 0x83A0
  lfs f6, 0(r6)
  fmuls f5, f6, f5
  fctiwz f5, f5
  stfd f5, 48(r2)
  lhz r12, 54(r2)
  stw r12, 0(r5)
  fctiwz f30, f30
  stfd f30, 16(r2)
  lhz r12, 22(r2)
  lfd f0, -31632(r2)
  lis r3, 0x4330
  ori r3, r3, 0x0
  stw r3, 16(r2)
  xoris r12, r12, 32768
  stw r12, 20(r2)
  lfd f30, 16(r2)
  fsub f30, f30, f0
  fadds f31, f31, f1
  fdivs f31, f31, f30
  b %end%


applyFlash:
  #Set r0 to color. First 6 digits are color, last 2 digits are opacity.
  lwz r3, 0xD8(r31)
  lwz r3, 0xAC(r3)

  #initial colour
  addi r4, r1, 0x18
  stw r0, 0(r4)

  li r5, 1
  lwz r12, 0(r3)
  lwz r12, 0x24(r12)
  mtctr r12
  bctrl

  lwz r3, 0xD8(r31)
  lwz r3, 0xAC(r3)
  #time to transition
  lis r0, 0x40C0
  stw r0, 0x18(r1)
  lfs f1, 0x18(r1)
  lis r0, 0xFFFF		  #target color of transition
  ori r0, r0, 0xFF00
  addi r4, r1, 0x18
  stw r0, 0(r4)
  #true
  li r5, 1

  lwz r12, 0x0(r3)
  lwz r12, 0x28(r12)
  mtctr r12
  bctrl
  mtlr r11
  blr
}
