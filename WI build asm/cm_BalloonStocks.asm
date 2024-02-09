######################################################
Smashville Balloon gives 1 stock to its attacker [Eon]
######################################################
# Code menu variant by mawwwk

.include Source/Extras/Macros.asm

.alias CodeMenuLoc = 0x804E02F0

HOOK @ $80979BE4 #at the `return True` of `onDamage/[ykDamageModuleImpl]/yk_damage_module_impl.o`
{
    lwz r3, 0x70(r31)   #get Module 
    lwz r3, 0(r3)       #getModuleName
    lwz r3, 0(r3)
    lis r4, 0x6772      #"gr"
    ori r4, r4, 0x5669  #"Vi"
    cmpw r3, r4         #if obj-type.startsWith("grVi") (to find "grVillageBalloon")
    bne end             #else end 
    # is a balloon

    #this.Modules.soDamage.getAttackerInfo()
    lwz r3, 0x60(r31)
    lwz r3, 0xD8(r3) 
    lwz r3, 0x38(r3)
    lwz r12, 0x8(r3)
    lwz r12, 0x94(r12)
    mtctr r12
    bctrl 
    mr r30, r3
	
    #TaskScheduler.getInstance()
    %call(0x8002E30C)
	
    #taskScheduler.getTaskById(attackerType, attackerID)
    lbz r4, 0x10(r30)
    lwz r5, 0x14(r30)
	%call(0x8002F018)
    cmpwi r3, 0
    beq end
	
    #attacker.getOwner()
    lwz r12, 0x3C(r3)
    lwz r12, 0x2EC(r12)
    mtctr r12
    bctrl 
    mr r30, r3
	
    #attackerOwner.getStockCount()
	%call(0x8081C540)
	
	%cmHeader(r12, CodeMenuLoc)
	cmpwi r12, 0			# If toggle off, do nothing
	beq end	
	
	cmpwi r12, 1			# If set to 1, add stock
	beq AddStock
	
	subi r4, r3, 1			# Else, subtract a stock
	b return
	
    #attackerOwner.setStockCount(prevStockCount+1)
	AddStock:
      addi r4, r3, 1
	
	return:
      mr r3, r30
	  %call(0x8081C528)
end:
    li r3, 1 
}