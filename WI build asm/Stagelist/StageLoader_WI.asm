########################################################
Custom Stage SD File Loader (WI ver.) [DukeItOut, mawwwk]
# WI build edit: Checks for _WI suffix in param files, and replaces based on Theme toggle
# Used in Source/Project+/StageFiles.asm
# Requires CMM SD File Saver
#
# Prerequisite: Stage ID in r3 (retrieves input, itself)
########################################################
CODE @ $8053E000
{
	lhz r0, 0xFB8(r12)
	cmpw r3, r0
	beqlr-					# Don't reload multiple times!
	sth r3, 0xFB8(r12)
	stwu r1, -0x130(r1)
	mflr r0
	stw r0, 0x134(r1)
	addi r11, r1, 0x50
	lis r12, 0x803F 		# \
	ori r12, r12, 0x12EC	# |
	mtctr r12				# | preserve registers r14-r31
	bctrl					# /
	lis r12, 0x8053
	ori r12, r12, 0xF000
	lis r28, 0x800B 
	ori r28, r28, 0x9EA0
	sth r3, 0(r28)			# Store stage ID for future reference
	mr r7, r3				# Stage ID
	li r0, 0xFF				# Clear random reroll flag for substages
	stb r0, -0x7E(r12)		#
	sth r7, -0x80(r12)		# Store stage ID
	addi r3, r1, 0x90
	lwz r4, -0x20(r12)		# "%s%s%02X%s"
	lwz r5, -0x1C(r12)		# "/stage/"
	lwz	r6, -0x18(r12)		# "stageslot/" 
	lwz	r8, -0x10(r12)		# ".asl"
	lis r12, 0x803F			# \
	ori r12, r12, 0x89FC	# | Create the filename string
	mtctr r12				# |
	bctrl					# /	
	lis  r5, 0x8053			# Stage files to 8053F000
	ori  r5, r5, 0xF000		#
	addi r3, r1, 0x60
	addi r4, r1, 0x90
	li r6, 0x0
	li r7, 0x0
	lis r12, 0x8002			# \
	ori r12, r12, 0x239C	# | set the read parameter
	mtctr r12				# |
	bctrl 					# /
	addi r3, r1, 0x60
	li r18, 0
	li r19, 0
	lis r12, 0x8001			# \
	ori r12, r12, 0xBF0C	# | load the file
	mtctr r12				# | 
	bctrl 					# /

	lhz		r16, 2(r28)			# / Get the input assigned
	lhz 	r3, 0(r28)			# Get access to the stage ID
    lhz     r17, -4(r28)      #\ 
    cmplwi  r17, 0x815E       # |
    bne-    ClassicAllStar    # | Replays require different info
    lhz     r17, -2(r28)      # |
    cmplwi  r17, 0x83E4       # |
    bne+    Multiplayer       #/
Replay:
	li r3, 11					# \
	bla 0x0249CC				# / Get replay heap offset
	lhz 	r16, 0x44A(r3)		# \ Get Replay ASL value	
	sth     r16, 2(r28)       	# /
	li		r0, 0xFF			# r0 was overwritten, but we still need it!
Multiplayer:    
ClassicAllStar:	
	lis r12, 0x8053
	ori r12, r12, 0xF000
	lhz r23, 4(r12)			# \ Get the slot count
	mtctr r23 				# /
	addi r23, r12, 8		# Get to the list of inputs and title offsets
	li r26, 0				# Choice-checking
	li r29, 0				# Most accurate choice, defaulting to the start
	li r21, 0				# How many inputs it shares
	mr r27, r16				# The input to check against
loop:	
	lhzx r25, r23, r26
	and. r25, r27, r25
	beq- not_found
found:
	mfctr r0				# Preserve the stage loop
	li r30, 16
	mtctr r30
	li r30, 1
	li r7, 0
checkPass:
	and. r24, r25, r30
	slwi r30, r30, 1
	beq bitSet
	addi r7, r7, 1
bitSet:
	bdnz checkPass
	mtctr r0				# Return the stage loop
	cmpw r7, r21			# \ If it doesn't match more than the previous best, skip
	ble+ not_found			# /
	mr r21, r7				# The amount of inputs shared by the new highest
	mr r29, r26				# Most accurate input offset 
not_found:	
	addi r26, r26, 4
	bdnz+ loop
	addi r29, r29, 0xA		# \ pass the 8-byte header and add 2 to get the offset instead of input
	lhzx r29, r12, r29		# /
	lhz r23, 6(r12)			# \ Get offset to param file names
	add r23, r23, r12		# /
	add r23, r23, r29		# Get the title address
	addi r3, r1, 0x90

#############################
### START WI LOADER EDITS ###
#############################
# $8053e174
.alias ThemeToggleLoc = 0x80495D1C
.alias SmashvilleID = 0x21
.alias PS2ID = 0x2E
.alias ConfigID = 0x26
.alias CxToggle = 1
.alias WaveToggle = 2
.alias InvToggle = 3

.alias addrHi = ThemeToggleLoc / 0x10000
.alias addrLo = ThemeToggleLoc & 0xFFFF

	lis r8, addrHi; ori r8, r8, addrLo
	lbz r8, 0(r8)			# Check theme toggle
	cmpwi r8, 0				# Skip this code if on default WI theme
	beq end_WI
	
	mr r7, r23				# Copy param filename loc

# Shortcut to avoid redundant param files
ConfigParamCheck:
	lis r6, 0x8053			# Load ASL ID
	ori r6, r6, 0xEF81
	lbz r6, 0(r6)
	cmpwi r6, ConfigID		# Check asl ID for menu (Config)
	bne SVParamCheck
	cmpwi r8, InvToggle		# Skip checking Config for IN and up
	bge end_WI
	addi r7, r7, 6			# "Config"
	b GetNewString			# No other possible suffixes, so replace

SVParamCheck:
	cmpwi r6, SmashvilleID	# \
	bne PS2ParamCheck		# | If Smashville and not Inv6/CV,
	cmpwi r8, InvToggle		# | skip checking params
	beq continueSV			# |
	cmpwi r8, CxToggle		# |
	bne end_WI				# /
continueSV:
	addi r7, r7, 10			# "Smashville"
	b StartCompare			# Check for other suffixes

PS2ParamCheck:
	cmpwi r6, PS2ID			# \ If PS2 and not Inv6,
	bne StartCompare		# | skip checking params
	cmpwi r8, InvToggle		# |
	bne end_WI				# /
	addi r7, r7, 17			# "Pokemon_Stadium_2"
	b StartCompare			# Check for other suffixes (_S, _DPAD)

StartCompare:
	lis r4, 0x5F57; ori r4, r4, 0x4900	# "_WI" followed by null terminator

CheckParamFilename:
	lwz r6, 0(r7)			# Compare param filename with "_WI."
	cmpw r6, r4
	beq GetNewString
	andi. r6, r6, 0xFF		# If terminator char reached, 
	beq end_WI				# give up
	
	addi r7, r7, 1			# Otherwise, check the next character
	b CheckParamFilename

# Load replacement string
GetNewString:
	cmpwi r8, CxToggle		# 1: "CX"
	li r5, 0x4358
	beq StoreString
	cmpwi r8, WaveToggle	# 2: "WV"
	li r5, 0x5756
	beq StoreString
	cmpwi r8, InvToggle		# 3: "IN"
	li r5, 0x494E
	beq StoreString
	li r5, 0x4356			# 4: "CV"

StoreString:
	sth r5, 1(r7)			# Replace "WI" with new suffix

end_WI:

###########################
### END WI LOADER EDITS ###
###########################

	lis r12, 0x8053			# Stage files write to 8053F000
	ori r12, r12, 0xF000	#	
	lis r4, 0x8048			#
	ori r4, r4, 0xEFF4		# %s%s%s%s	
	lwz r5, -0x1C(r12)		# "/stage/"
	lwz	r6, -0x14(r12)		# "stageinfo/" 
	mr	r7, r23				# <-r23 filename 
	lwz	r8, -0xC(r12)		# ".param"
	lis r12, 0x803F			# \
	ori r12, r12, 0x89FC	# | Create the filename string
	mtctr r12				# |
	bctrl					# /	
	
	lis  r5, 0x8053		#    \ Stage lists are written to 8053F200
	ori  r5, r5, 0xF000	# ** /
	addi r3, r1, 0x60
	addi r4, r1, 0x90
	li r6, 0
	li r7, 0
	lis r12, 0x8002			# \
	ori r12, r12, 0x239C	# | set the read parameter
	mtctr r12				# |
	bctrl 					# /
	addi r3, r1, 0x60
	li r18, 0
	li r19, 0
	lis r12, 0x8001			# \
	ori r12, r12, 0xBF0C	# | load the file
	mtctr r12				# | 
	bctrl 					# /

	lis r12, 0x8053			# \
	ori r12, r12, 0xF000	# | Get the tracklist file name
	lwz r8, 0x18(r12)		# |
	lwz r4, 0x4(r12)		# |
	add r4, r4, r12			# |
	add r8, r8, r4			# /
	
TracklistLoading:	
	stwu r1, -0xF0(r1)
	stw r4, 0x8(r1)
	stw r5, 0xC(r1)
	stw r6, 0x10(r1)
	stw r7, 0x14(r1)
	stw r3, 0x18(r1)
	lis r4, 0x8053;  ori r4, r4, 0xCFF8	# 
	lwz r5, 0x0(r4)						# "sound/tracklist/" (or netplaylist)
	lwz r4, 0x4(r4)						# "%s%s.tlst"
	mr r6, r8
	addi r3, r1, 0x60
	lis r12, 0x803F				# \
	ori r12, r12, 0x89FC		# | Create the filename string
	mtctr r12					# |
	bctrl						# /
	lis  r5, 0x8053				# Tracklists are written to 8053F200, stage files to 8053F000
	ori  r5, r5, 0xF200			#
	addi r3, r1, 0x30
	addi r4, r1, 0x60
	li r6, 0x0
	li r7, 0x0
	lis r12, 0x8002			# \
	ori r12, r12, 0x239C	# | set the read parameter
	mtctr r12				# |
	bctrl 					# /
	addi r3, r1, 0x30
	li r6,0					# Necessary to prevent a max filesize override by the File Patch Code!
	lis r12, 0x8001			# \
	ori r12, r12, 0xBF0C	# | load the file
	mtctr r12				# | 
	bctrl 					# /
	lwz r4, 0x8(r1)
	lwz r5, 0xC(r1)
	lwz r6, 0x10(r1)
	lwz r7, 0x14(r1)
	lwz r3, 0x18(r1)
	addi r1, r1, 0xF0	

FinishedTracklistLoading:
	
	addi r11, r1, 0x50
	lis r12, 0x803F			# \
	ori r12, r12, 0x1338	# |
	mtctr r12				# | Restore registers r14-r31
	bctrl					# /
	lwz r0, 0x134(r1)
	mtlr r0
	lwz r1, 0(r1)
	blr
}