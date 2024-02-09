########################################################
Custom Stage SD File Loader (WI ver.) [DukeItOut, mawwwk]
# WI build edit: Checks for _WI suffix in param files, and replaces based on Theme toggle
# Uses addresses:
# 8053EF10: Writes Splat battle tlst string loc
# 8053EF14: Stores currently opened TLST name (first 4 chars)
# Used in Source/Project+/StageFiles.asm
# Requires CMM SD File Saver
#
# Prerequisite: Stage ID in r3 (retrieves input, itself)
########################################################
.include Source/Extras/Macros.asm

	.BA<-SplatBattleTLST
	.BA->$8053EF10
	.RESET
.GOTO->LoaderCode
SplatBattleTLST:
	string "SP_Battle"

LoaderCode:
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
	lhz r4, 2(r28)			# \ (NEW P+ 2.5: )
	sth r4, -0x46(r12)		# / Store stage ASL ID
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
# $8053e0f0
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

############################
### START WI EDITS (1/2) ###
############################
# $8053e17c
.alias ThemeToggleLoc = 0x80495D1C
.alias ConfigID = 0x26
.alias ResultsID = 0x28
.alias CxToggle = 1
.alias CvToggle = 2
.alias SpToggle = 3
.alias WvToggle = 4
.alias Inv6Toggle = 5	# \ PS2, YI, TOT CHECKS ASSUME THIS ORDER
.alias Inv7Toggle = 6	# /

	%lwi(r12, ThemeToggleLoc)	# r12: Theme addr
	%loadByte(r6, 0x8053EF81)	# r6: ASL stage ID
	mr r7, r23					# r7: Param filename loc
	lbz r8, 0(r12)				# r8: Theme ID

	cmpwi r6, ResultsID
	bne notResults

ResultsCodeMenuCheck:
	addi r7, r7, 7			# "Results"
	
	lbz r5, 2(r12)					# Check code menu Results toggle
	cmpwi r5, 1; beq GetThemeString # Results: Theme
	cmpwi r5, 2; beq StageResults 	# Results: Stage
	cmpwi r5, 3; beq end_WI			# Results: Skip
	
							# \ Default results behavior:
	cmpwi r8, 0             # | If WI theme, use stage-specific results.
	bne GetThemeString      # / Otherwise load theme-based results param

StageResults:
	%loadByte(r6, 0x9017F42D)	# Load previous stage ID
	
	cmpwi r6, 0x01; li r5, 0x4246; beq StoreString	# Battlefield
	cmpwi r6, 0x02; li r5, 0x4644; beq StoreString	# Final Destination
	cmpwi r6, 0x04; li r5, 0x4C4D; beq StoreString	# Luigi's Mansion
	cmpwi r6, 0x09; li r5, 0x5454; beq StoreString	# Temple of Time
	cmpwi r6, 0x0C; li r5, 0x4648; beq StoreString	# Frigate Husk
	cmpwi r6, 0x0D; li r5, 0x5949; beq StoreString	# Yoshi's Island
	cmpwi r6, 0x21; li r5, 0x5356; beq StoreString	# Smashville
	cmpwi r6, 0x2D; li r5, 0x444C; beq StoreString	# Dream Land
	cmpwi r6, 0x37; li r5, 0x5452; beq StoreString	# Training Room
	cmpwi r6, 0x47; li r5, 0x4754; beq StoreString	# Golden Temple
	cmpwi r6, 0x2E	# PS2
	
	bne GetThemeString		# If stage ID not found, use theme

PS2_Results:
	li r5, 0x5053			# Use "PS"
	%lwi(r12, 0x8053EFBA)	# Get ASL ID
	lhz r12, 0(r12)
	andi. r12, r12, 0x1000	# Check if Start is pressed
	beq StoreString			#
	li r5, 0x4D53			# If so, use "MS"
	b StoreString

notResults:
	cmpwi r8, 0				# If theme is WI,
	beq end_WI				# skip this code (no stage change)
	
	cmpwi r6, ConfigID		# Check asl ID for menu (Config)
	bne StageCheck
	cmpwi r8, Inv6Toggle	# \ If Config and not Inv6/CV,
	beq end_WI				# | skip checking params
	cmpwi r8, CvToggle		# | (These use Config_WI)
	beq end_WI				# /
	addi r7, r7, 6			# "Config"
	b GetThemeString		# No other possible suffixes, so replace

StageCheck:					# Not on menu or results
	cmpwi r8, CxToggle		# \ If Construct theme,
	beq DelfinoParamCheck	# / force a continue to check stages
	
	lbz r5, 1(r12)			# \ If Alternate Stages OFF,
	cmpwi r5, 2				# | skip this code
	beq end_WI				# /

DelfinoParamCheck:
	cmpwi r6, 0x03			# \
	bne MetalParamCheck		# | If Delfino and not Inv6,
	cmpwi r8, Inv6Toggle	# | skip checking params
	bne end_WI				# /
	addi r7, r7, 14			# "Delfino_Secret"
	b StartCompare

MetalParamCheck:
	cmpwi r6, 0x05			# \
	bne SVParamCheck		# | If MC and not Inv6,
	cmpwi r8, Inv6Toggle	# | skip checking params
	bne end_WI				# /
	addi r7, r7, 12			# "Metal_Cavern"
	b StartCompare

SVParamCheck:
	cmpwi r6, 0x21			# \
	bne PS2ParamCheck		# | If Smashville and not Inv6/CX,
	cmpwi r8, Inv6Toggle	# | skip checking params
	beq continueSV			# |
	cmpwi r8, CxToggle		# |
	bne end_WI				# /
continueSV:
	addi r7, r7, 10			# "Smashville"
	b StartCompare

PS2ParamCheck:
	cmpwi r6, 0x2E			# \ If PS2 and not Inv6 or Inv7,
	bne TOTParamCheck		# | skip checking params
	cmpwi r8, Inv6Toggle	# |
	blt end_WI				# /
	addi r7, r7, 17			# "Pokemon_Stadium_2"
	b StartCompare

TOTParamCheck:
	cmpwi r6, 0x09			# \
	bne YIParamCheck		# | If Temple of Time and not Inv6 or Inv7,
	cmpwi r8, Inv6Toggle	# | skip checking params
	blt end_WI				# /
	addi r7, r7, 14			# "Temple_of_Time"
	b StartCompare

YIParamCheck:
	cmpwi r6, 0x0D			# \
	bne StartCompare		# | If YI and not Inv6 or Inv7,
	cmpwi r8, Inv6Toggle	# | skip checking params
	blt end_WI				# /
	addi r7, r7, 13			# "Yoshis_Island"

StartCompare:
	lis r4, 0x5F57; ori r4, r4, 0x4900	# "_WI" followed by null terminator

CheckParamFilename:
	lwz r6, 0(r7)			# \ Compare param filename with "_WI."
	cmpw r6, r4				# /
	beq GetThemeString
	andi. r6, r6, 0xFF		# If terminator char (00) reached,
	beq end_WI				# give up
	
	addi r7, r7, 1			# Otherwise, check the next character
	b CheckParamFilename

# Load replacement theme string. Location must be set in r7
GetThemeString:
	cmpwi r8, 0; li r5, 0x5749; beq StoreString				# "WI"
	cmpwi r8, CxToggle; li r5, 0x4358; beq StoreString		# "CX"
	cmpwi r8, CvToggle; li r5, 0x4356; beq StoreString		# "CV"
	cmpwi r8, SpToggle; li r5, 0x5350; beq StoreString		# "SP"
	cmpwi r8, WvToggle; li r5, 0x5756; beq StoreString		# "WV"
	cmpwi r8, Inv6Toggle; li r5, 0x4936; beq StoreString	# "I6"
	li r5, 0x4937			# "I7"

StoreString:
	sth r5, 1(r7)			# Replace "WI" with new suffix

end_WI:

##########################
### END WI EDITS (1/2) ###
##########################
# $8053e380
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
############################
### START WI EDITS (2/2) ###
############################
# $8053e338
	%lwi(r12, 0x8053F000)	# \ Get tracklist file name
	lwz r8, 0x18(r12)		# |
	lwz r4, 0x4(r12)		# |
	add r4, r4, r12			# |
	add r8, r8, r4			# /
	
	%lwi(r18, 0x8053EF10)		# SP_Battle tlst loc
	%lwi(r12, ThemeToggleLoc)
	
	lbz r5, 0(r12)				# \ If not on splat theme,
	cmpwi r5, SpToggle     		# | load tracklist from param
	bne TracklistLoading    	# /
	
	lbz r5, 1(r12)				# Check Alternate Stage toggle
	cmpwi r5, 2					# If OFF,
	beq TracklistLoading		# load tracklist from param
	
	%loadByte(r12, 0x8053EF81)	# Load ASL ID
	cmpwi r12, ConfigID			# If on Config or Results,
	beq TracklistLoading        # load tracklist from param
	cmpwi r12, ResultsID
	beq TracklistLoading
	
	lhz r12, 2(r28)				# Check controller inputs at 800b9ea2
	andi. r12, r12, 0xF			# \ If between 0x1-0xF inclusive,
	beq LoadSpBattleTLST		# | dpad is held, so use normal tlst
	cmpwi r12, 0xF				# |
	ble TracklistLoading		# /

LoadSpBattleTLST:	
	lwz r8, 0(r18)		# Use Sp_Battle tracklist

TracklistLoading:
	lwz r19, 0(r8)		# Store tracklist name in 8053EF14
	stw r19, 4(r18)		# for checks in My Music

##########################
### END WI EDITS (2/2) ###
##########################

	stwu r1, -0xF0(r1)
	stw r4, 0x8(r1)
	stw r5, 0xC(r1)
	stw r6, 0x10(r1)
	stw r7, 0x14(r1)
	stw r3, 0x18(r1)
	lis r4, 0x8053;  ori r4, r4, 0xCFF8	# $8053CFF8
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