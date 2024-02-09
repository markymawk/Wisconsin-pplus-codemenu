# Stagelist: Default (14 stages)
* 26495D1B 00000001 # If Stagelist value < 1

byte 14 @ $806B929C # Page 1
byte 27 @ $806B92A4 # Page 2
byte 68 @ $800AF673 # Stage Count (sum of above values plus 27)

.BA<-STG0_PAGE1
.BA->$80495D04
.BA<-STG0_PAGE2
.BA->$80495D08
.GOTO->STG0_SkipStageTables

STG0_PAGE1:
    byte[14] |
0x0C, 0x18, 0x04, 0x0B, 0x21, 0x01, 0x23, | # YI, FoD, Metal, Frigate, GT, FD, DL
0x1C, 0x08, 0x1A, 0x00, 0x28, 0x03, 0x02 # GHZ, ToT, SV, BF, PS2, Mansion, Delfino

STG0_PAGE2:
    byte[27] |
0x0E, | # Lylat
0x1B, | # Shadow
0x19, | # New Pork
0x0D, | # Halberd
0x36, | # Port Town
0x3B, | # Mario Circuit
0x2E, | # Eldin
0x38, | # Mushroomy Kingdom
0x45, | # Flat Zone
0x2F, | # Hanenbow
0x07, | # Rumble
0x10, | # Spear
0x35, | # Norfair
0x39, | # WarioWare
0x17, | # Skyworld
0x09, | # Pirate Ship
0x3D, | # Delfino Plaza
0x1D, |	# Picto
0x2B, | # Training
0x16, | # DP
0x14, | # Siege
0x05, | # Bowser
0x15, | # Wario Land
0x12, | # Glacier
0x34, | # Skyloft
0x44, |	# Bobomb
0x3A | # Subspace

STG0_SkipStageTables:
.RESET

* E0000000 80008000