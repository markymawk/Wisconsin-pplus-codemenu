.include Source/Stagelist/Stagelist0.asm	# Default
.include Source/Stagelist/Stagelist1.asm	# P+ 2023 Singles
.include Source/Stagelist/Stagelist2.asm	# WI 2023 Doubles (GT, no FH)
.include Source/Stagelist/Stagelist3.asm	# P+ 2023 Doubles (FH, no GT)
.include Source/Stagelist/Stagelist4.asm	# PMBR Singles
.include Source/Stagelist/Stagelist5.asm	# PMBR Doubles

.BA<-TABLE_STAGES
.BA->$80495D00
.BA<-TABLE_3
.BA->$80495D0C
.BA<-TABLE_4
.BA->$80495D10
.BA<-TABLE_5
.BA->$80495D14
.GOTO->SkipStageTablesBase

TABLE_3:
    byte[27] |
0x11, | # Hammer Express
0x41, | # Homerun Stadium
0x43, | # Lunar Horizon
0x1F, | # Sprout Tower
0x32, | # Temple Tempest
0x1E, | # Sky Sanctuary
0x42, | # Oil Drum
0x2C, | # Dracula
0x40, | # Cookie Country
0x13, | # QBert
0x3C, | # Corneria
0x37, | # Great Bay
0x25, | # Poke Floats
0x33, | # Jungle Japes
0x20, | # Yoshi's Story
0x31, | # Dinosaur Land
0x26, | # Big Blue
0x2D, | # Green Greens
0x0A, | # King of the Hill
0x27, | # Planet Zebes
0x3E, | # Hyrule Castle
0x0F, | # Saffron City
0x06, | # Kongo Jungle 64
0x30, | # Super Happy Tree
0x24, | # Peach's Castle
0x3F, | # Mute City
0x22  | # Onett

TABLE_4:	# Unused
TABLE_5:	# Unused

TABLE_STAGES:
# Table of icon<->stage slot associations.
# 0x3C and 0x40 are reserved for Target Smash replay cosmetics. avoid these
half[70] |  # Stage Count + 2
| # OLD SLOTS
0x0101, 0x0202, 0x0303, 0x0404, | # Battlefield, Final Destination, Delfino's Secret, Luigi's Mansion
0x0505, 0x0606, 0x0707, 0x0808, | # Metal Cavern, Bowser's Castle, Kongo Jungle, Rumble Falls
0x091A, 0x3309, 0x492C, 0x0C0C, | # Pirate Ship, Temple of Time, King, Meadows
0x0D0D, 0x0E0E, 0x130F, 0x1410, | # Yoshi's Island, Halberd, Lylat Cruise, Saffron City
0x1511, 0x162D, 0x1713, 0x1814, | # Spear Pillar, Hammer Express, Infinite Glacier, QBert
0x1915, 0x1C16, 0x1D17, 0x1E18, | # Castle Siege, Wario Land, Distant Planet, Skyworld
0x1F19, 0x2048, 0x211B, 0x221C, | # Fountain of Dreams, New Pork, Smashville, Shadow Moses Island
0x231D, 0x241E, 0x4326, 0x2932, | # Green Hill Zone, PictoChat, Sky Sanctuary, Sprout Tower
0x2A33, 0x472A, 0x2C35, 0x2D36, | # Yoshi's Story, Golden Temple, Onett, Dream Land
0x2F37, 0x5049, 0x3139, 0x323A, | # Peach's Castle, Poke Floats (moved), Big Blue, Zebes
0x2E3B, 0xFF64, 0xFF64, 0x373C, | # Pokemon Stadium 2, NOTHING, NOTHING, Training Room
0x4023, 0x4124, 0x4225, 0x251F, | # Dracula's Castle, Green Greens, Bridge of Eldin, Hanenbow
0x4427, 0x4528, 0x4629, 0x2B34, | # Super Happy Tree, Dinosaur Land, Temple Tempest, Jungle Japes
0x482B, 0x0B0B, 0x4A12, 0x4B2E, | # Skyloft, Norfair, Port Town, Great Bay
0x4C2F, 0x4D30, 0x4E31,         | # Mushroomy Kingdom, WarioWare, Subspace
| # WI added slots
0x4F3D, 0x3038, 0x513F, 0x520A, | # Mario Circuit, Corneria (moved), Delfino Plaza, Hyrule Castle
0x5341, 0x5442, 0x5543, 0x5644, | # Mute City, Cookie Country, Homerun Stadium, Oil Drum
0x5745, 0x5846, 0x5947 # Lunar Horizon, Bobomb, Flat Zone

SkipStageTablesBase:
.RESET

byte 27 @ $80496002 # Page 3
byte 00 @ $80496003 # Page 4 (Unused)
byte 00 @ $80496004 # Page 5 (Unused)