###################################
Enable fastfall on aerial hit [Eon]
###################################
# Code menu variant by mawwwk

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02D8
.alias fallFrameBuffer = 5

HOOK @ $8077e8d4                # "First hook is innocuous on its own"
{
  stw r3, 0x10(r1)
  cmpwi r3, 0
}

HOOK @ $8077e8f4
{  
    lwz r31, 0x1C(r1)           # Original instr
	%cmHeader(r4, CodeMenuLoc)
    cmpwi r4, 0                 # / Skip if toggled off
    beq end
    
    lwz r3, 0x10(r1)            # If hitstun/SDI-able code, just jump out its not worth it
    %lwi(r4, 0x80B897BC)
    cmpw r3, r4
    beq end

    lwz r3, 0xD8(r30)
    lwz r3, 0x5C(r3)
    lwz r12, 0x0(r3)
    lwz r12, 0x6C(r12)
    mtctr r12 
    bctrl                       # getFlickY 
    cmpwi r3, fallFrameBuffer   # Input window for tap input
    bge end
    
    lwz r3, 0xD8(r30)
    lwz r3, 0x5C(r3)
    lwz r12, 0x0(r3)
    lwz r12, 0x50(r12)
    mtctr r12 
    bctrl #getStickY
    lis r3, 0xbf1A
    stw r3, 0x1C(r1)
    lfs f2, 0x1C(r1)
    fcmpo cr0, f1, f2       # If stick not below threshold this frame
    bgt end                 # stop
    
    #set fastfall bit, if the move can fastfall, it will, even if you normally never would
    lwz r3, 0xD8(r30)
    lwz r3, 0x64(r3)
    lis r4, 0x2200          #ra-bit
    ori r4, r4, 0x2 #2
    lwz r12, 0x0(r3)
    lwz r12, 0x50(r12)
    mtctr r12 
    bctrl                   #onFlag   
    
end:
    lwz r0, 0x24(r1)
}