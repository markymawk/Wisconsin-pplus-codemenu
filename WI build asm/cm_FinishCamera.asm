#########################################
!Melee-style camera freeze on game finish [Eon, toggle ver. by mawwwk]
# Not included or tested for WI build, but maybe later
#########################################
.alias CodeMenuLoc = ??

End game screen freezes [Eon]
HOOK @ $8009CAB8 {
	%cmHeader(r30, CodeMenuLoc)
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
	%cmHeader(r3, CodeMenuLoc)
	cmpwi r3, 0
	beq SlowMotionCamera_2
	lis r3, 0x805A
	lwz r3, -0x54(r3) //gfApplication
	li r4, 1 //enable pause
	li r5, 0 //pause type 0
	%call(0x80016900)	//setPause
	b %END%
SlowMotionCamera_2:
	li r3, 4
}