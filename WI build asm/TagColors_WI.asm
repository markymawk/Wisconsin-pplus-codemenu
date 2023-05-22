###########################################
WI theme-specific tag rumble color [mawwwk, original code by Eon]
###########################################
# Used in Source/LegacyTE/TagsControls.asm
# Base code by Eon, with credit to chasemcdizzle, Fracture, Yohan1044

.alias ThemeToggleLoc = 0x80495D1C
.alias WI_tagColor = 0x43255F
.alias CX_tagColor = 0x1E541A
.alias WV_tagColor = 0x6F2A6C
.alias IN_tagColor = 0x1A5454
.alias CV_tagColor = 0x2B4A87

.macro loadThemeSetting(<reg>)
{
	.alias addrHi = ThemeToggleLoc / 0x10000
	.alias addrLo = ThemeToggleLoc & 0xFFFF
	
	lis <reg>, addrHi
	ori <reg>, <reg>, addrLo
	lbz <reg>, 0(<reg>)
}

.macro compareThemeSetting(<reg>)
{
	cmpwi <reg>, 0; beq setColorWI
	cmpwi <reg>, 1; beq setColorCX
	cmpwi <reg>, 2; beq setColorWV
	cmpwi <reg>, 3; beq setColorIN
	# Else use CV
}

.macro storeTagColor(<hexColor>, <darkFactor>, <rReg>, <gReg>, <bReg>)
{
	.alias darken = <darkFactor> * 0x0B0B0B
	.alias finalColor = <hexColor> - darken
	.alias red = <hexColor> / 0x10000
	.alias greenTemp = <hexColor> & 0xFF00
	.alias green = greenTemp / 0x100
	.alias blue = <hexColor> & 0xFF
	
	li <rReg>, red
	li <gReg>, green
	li <bReg>, blue
	
	b end
}

# Selected tag
HOOK @ $8069fa0c
{
	li r18, 0
	cmpwi r16, 1
	bne %end%
	%loadThemeSetting(r3)
	%compareThemeSetting(r3)
	%storeTagColor(CV_tagColor, 0, r20, r19, r18)
	
setColorWI:
	%storeTagColor(WI_tagColor, 0, r20, r19, r18)

setColorCX:
	%storeTagColor(CX_tagColor, 0, r20, r19, r18)

setColorWV:
	%storeTagColor(WV_tagColor, 0, r20, r19, r18)

setColorIN:
	%storeTagColor(IN_tagColor, 0, r20, r19, r18)

end:
}

# Unselected tag	
HOOK @ $8069fa1c	
{	
	li r18, 0x50	
	cmpwi r16, 1	
	bne %end%
	%loadThemeSetting(r3)
	%compareThemeSetting(r3)
	%storeTagColor(CV_tagColor, 1, r20, r19, r18)
	
setColorWI:
	%storeTagColor(WI_tagColor, 1, r20, r19, r18)

setColorCX:
	%storeTagColor(CX_tagColor, 1, r20, r19, r18)

setColorWV:
	%storeTagColor(WV_tagColor, 1, r20, r19, r18)

setColorIN:
	%storeTagColor(IN_tagColor, 1, r20, r19, r18)

end:
}

# Changing from selected to unselected	
HOOK @ $806a05c8 	
{
	li r8, 0xFF	
	cmpwi r17, 1
	bne %end%
	%loadThemeSetting(r4)
	%compareThemeSetting(r4)
	%storeTagColor(CV_tagColor, 2, r5, r6, r7)
	
setColorWI:
	%storeTagColor(WI_tagColor, 2, r5, r6, r7)

setColorCX:
	%storeTagColor(CX_tagColor, 2, r5, r6, r7)

setColorWV:
	%storeTagColor(WV_tagColor, 2, r5, r6, r7)

setColorIN:
	%storeTagColor(IN_tagColor, 2, r5, r6, r7)

end:
	mr r4, r30	# Restore register
}
