#########################################
!Melee-style camera freeze on game finish [Eon, toggle ver. by mawwwk]
# Not included or tested for WI 1.5, but maybe later
#########################################
.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = ??

End game screen freezes [Eon]
HOOK @ $8009CAB8 {
	lis r30, CodeMenuStart
	ori r30, r30, CodeMenuHeader
	cmpwi r30, 0
	beq SlowMotionCamera
	rlwinm r3, r3, 5, 27, 30
	b %END%
SlowMotionCamera:
	rlwinm, r3, r3, 5, 27, 30
}
# requires pause code above for camera to not freeze
HOOK @ $806d34ec
{
	lis r3, CodeMenuStart
	ori r3, r3, CodeMenuHeader
	cmpwi r3, 0
	beq SlowMotionCamera_2
	lis r3, 0x805A
	lwz r3, -0x54(r3) //gfApplication
	li r4, 1 //enable pause
	li r5, 0 //pause type 0
	lis r12, 0x8001 //setPause
	ori r12, r12, 0x6900
	mtctr r12
	bctrl
	b %END%
SlowMotionCamera_2:
	li r3, 4
}