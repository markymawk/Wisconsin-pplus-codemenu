########################################################################
Stagelist Looter Data  [Desi]
########################################################################
    .alias StagelistDataLocationHigh = 0x8049
    .alias StagelistDataLocationLow = 0x5D3C
PULSE
{
    lis r16, StagelistDataLocationHigh
    lwz r16, StagelistDataLocationLow (r16)
    blr
}
    .BA<-StagelistRoot
* 54010000 0000000C     #Store to Pointer Address
	.BA<-StagePacPath
* 54010000 00000010
	.BA<-StageModulePath
* 54010000 00000014
	.BA<-CSERoot
* 54010000 00000018
    .BA<-FileNameFolder
	.BA->$8053EFE4
	.BA<-FileNameLocation
	.BA->$8053EFE8
	.BA<-FileNameLocation2
	.BA->$8053EFEC	
	.BA<-FileNameLocation3
	.BA->$8053CFF8	
	.BA<-FileNameLocation4
	.BA->$8053EFB4
    .BA<-TABLE_STAGES
    .BA->$80495D00
    .BA<-TABLE_1
    .BA->$80495D04
    .BA<-TABLE_2
    .BA->$80495D08
    .BA<-TABLE_3
    .BA->$80495D0C
    .BA<-TABLE_4
    .BA->$80495D10
    .BA<-TABLE_5
    .BA->$80495D14
    .RESET
    .GOTO->SkipData

#Any SD Root must be 8 letters.
StagelistRoot:
	string "/Project+/"	#This SD Root applies to all Stagelist Related Files except BRSTMs.
CSERoot:
	string "/Project+/"	#This SD Root applies to BRSTMs. 
FileNameFolder:		
	string "/stage/"	#This is the prefix for FileNameLocation 1 and 2. SD Root is required for locations outside of the builds SD Root.
FileNameLocation:
	string "stageslot2/"
FileNameLocation2:
	string "stageinfo/"	
FileNameLocation3: 		#This is where the Tracklist Configs are loaded from. SD Root and pf are required for locations outside of the builds SD Root.
	string "/sound/tracklist/"
FileNameLocation4:		
	string "pf/sound/"
StagePacPath:			#Do not change /STAGE/MELEE/. It needs to be all caps. If you edit STG, beware that there is a file legnth limit on the Stage Title.
	string "/STAGE/MELEE/STG"
StageModulePath:		#This is the path used by Stage Modules. SD Root and pf are required for locations outside of the builds SD Root.
	string "DVD:/Project+/pf/module/"


TABLE_1:
	byte[11] |
0x04, 0x21, 0x01, 0x23, 		| # Metal, Golden Temple, FD, Dream Land
0x19, 0x1A, 0x00, 0x28, 0x02, 	| # Siege (alt), SV, BF, PS2, Delfino
0x2B, 0x2B						  # Jank training icons

TABLE_2:
	byte[29] |
0x12, |	# Glacier
0x1B, | # Shadow
0x0E, | # Lylat
0x14, | # Siege
0x0C, | # YIB
0x36, | # Port Town
0x2E, | # Eldin
0x03, |	# Luigi
0x2F, | # Hanenbow
0x13, | # Flat
0x07, | # Rumble
0x16, | # DP
0x10, | # Spear
0x39, | # WarioWare
0x08, | # Pirates
0x0B, | # Frigate
0x35, | # Norfair
0x1F, | # Skyloft
0x3A, | # Subspace
0x2B, | # Training
0x05, | # Bowser
0x20, | # Story
0x15, | # Wario
0x18, | # FOD
0x1C, | # GHZ
0x0A, | # Metroid Lab
0x0D, | # Halberd
0x17, | # Skyworld
0x1D	# Picto

TABLE_3:
	byte[24] |
0x3B, | # Heaven
0x37, | # Chamber
0x11, | # Hammer Express
0x32, | # Tempest
0x34, | # Bell
0x3D, | # Gorge
0x1E, | # SSZ
0x30, | # Dead Line 
0x2C, | # Dracula
0x2D, | # Bobomb
0x25, | # Corneria
0x33, | # Japes
0x3C, | # Floats
0x31, | # Dino Land
0x26, | # Big Blue
0x3E, | # Fourside
0x27, | # Zebes
0x24, | # Peach
0x09, | # HyCas
0x06, | # Kongo 64 
0x0F, | # Saffron
0x38, | # MK64
0x3F, | # Mute City
0x22  | # Onett

TABLE_4:	# Unused

TABLE_5:	# Unused

.include TABLE_STAGES.asm

SkipData:
byte 11 @ $806B929C # Page 1
byte 28 @ $806B92A4 # Page 2
byte 24 @ $80496002 # Page 3
byte 00 @ $80496003 # Page 4 (Unused)
byte 00 @ $80496004 # Page 5 (Unused)
byte 62 @ $800AF673 # Stage Count


PULSE       #Setup GCT Link Return
{
    lis r16, StagelistDataLocationHigh
	lwz r16, StagelistDataLocationLow (r16)
	lwz r16, 8 (r16)
    addi r16, r16, 0x30
    lis r6, 0x8000
    stw r16, 0x1848 (r6)
	blr
}

* 64000000 00000000     #GCT Link Return