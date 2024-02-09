# Stagelist: PMBR Doubles (8 stages)
* 20495D1B 00000005 # If Stagelist value == 5

byte 13 @ $806B929C # Page 1
byte 27 @ $806B92A4 # Page 2
byte 64 @ $800AF673 # Stage Count (sum of above values plus 27)

# Random stage select
* 42000000 90000000
* 0417BE70 00021000
* 0417BE74 14000017

.BA<-STG5_PAGE1
.BA->$80495D04
.BA<-STG5_PAGE2
.BA->$80495D08
.GOTO->STG5_SkipStageTables

STG5_PAGE1:
    byte[8] |
0x04, 0x01, 0x23, 			  | # Metal, FD, DL
0x1C, 0x1A, 0x00, 0x28, 0x02 # GHZ, SV, BF, PS2, Delfino

STG5_PAGE2:
    byte[27] |
0x0C, 0x0B, 0x21, 0x08, 0x03, | # YI, Frigate, GT, ToT, Mansion
0x18, | # FoD
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

STG5_SkipStageTables:
.RESET

* E0000000 80008000