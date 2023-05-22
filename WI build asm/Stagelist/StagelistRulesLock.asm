################################
Stagelist rules lock [mawwwk]
################################
.alias StagelistToggleLoc = 0x80495D1B

* 24495D1B 00000000			# If stagelist set,
* 42000000 90000000
* 0017F366 0000000A			# Damage ratio
* 0017F369 00000001			# Team attack off
* 0217F36B 00000000			# ALC off, input assist off
* 0017BE50 00000000			# Items NONE
* 0017BE59 0000000E			# \
* 0017BE5A 00050000			# / Items off
* 42000000 80000000
* E0000000 80008000

.macro CompareStagelistToggle(<reg>)
{
	.alias addrHi = StagelistToggleLoc / 0x10000
	.alias addrLo = StagelistToggleLoc & 0xFFFF
	lis <reg>, addrHi
	ori <reg>, <reg>, addrLo
	lbz <reg>, 0(<reg>)
	cmpwi <reg>, 0
	beq UpdateVal
}

#######################
# Lock menu rule options from scrolling
#######################

# Damage ratio lock
HOOK @ $806A79E8
{
	%CompareStagelistToggle(r25)
	li r0, 10; b %END%		# If stagelist set, lock damage ratio at 1.0

UpdateVal:
	add r0, r0, 24			# If default stagelist, run original op
}

# Timer lock (99:00 -> infinite -> 8:00)
HOOK @ $806A96D8
{
	%CompareStagelistToggle(r4)
	cmpwi r0, 98; bge maxTimer	# Cap leftmost at 99
	cmpwi r0, 9; bge eightMin	# Cap rightmost at 8
	b UpdateVal

eightMin:
	li r0, 8; b UpdateVal

maxTimer:
	li r0, 99				# 99 min, for crew battles mostly

UpdateVal:
	stb r0, 0x66C(r3)		# If default stagelist, run original op
}

# Team attack lock
HOOK @ $806A9900
{
	%CompareStagelistToggle(r4)
	li r4, 0; b %END%		# Treat "ON" as the left-most option
	
UpdateVal:
	lbz	r4, 0x66D(r3)		# If default stagelist, run original op
}

# ALC lock
HOOK @ $806A9D9C
{
	%CompareStagelistToggle(r4)
	li r4, 1; b %END%		# Treat "OFF" as the right-most ooption
	
UpdateVal:
	lbz r4, 0x66F(r3)		# If default stagelist, run original op
}

# Input assist lock
HOOK @ $806A9F90
{
	%CompareStagelistToggle(r4)
	li r4, 1; b %END%		# Treat "OFF" as the right-most option

UpdateVal:
	lbz r4, 0x670(r3)		# If default stagelist, run original op
}
