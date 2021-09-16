Reset Teams on re-enter CSS [Eon]
#Code Menu Variant by mawwwk
#Calls assignTeams if teams are enabled
.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = 0x02CC     #Offset of word containing location of code menu toggle
HOOK @ $80683634
{  
  mr r3, r30
  lis r12, 0x8068
  ori r12, r12, 0x4D84
  mtctr r12 
  bctrl 
  cmpwi r3, 1 #if is teams (if not go to vanilla code)
  bne end
  lis r3, CodeMenuStart			# Code menu check
  ori r3, r3, CodeMenuHeader
  lwz r3, 0(r3)
  lbz r3, 0xB(r3)
  cmpwi r3, 1                   # If toggle not set, use default behavior
  bne end                       #
  mr r3, r30 #assign random teams
  lis r12, 0x8068
  ori r12, r12, 0xAC4C
  mtctr r12 
  bctrl 
end:
  li r3, 42
}


Assign Teams randomises colour & order [Eon]
.macro randi(<i>)
{
  li r3, <i> 
  lis r12, 0x8003
  ori r12, r12, 0xfc7c
  mtctr r12 
  bctrl 
}
.alias totalTeams = 3 #assumes this is prime (since all values less than it create a loop len totalTeams when adding and modulo'ing)
.alias totalTeamsSubOne = totalTeams-1
HOOK @ $8068ACD0
{
  cmpwi r6, 1 #r6 = number of chars to give each team, but i want how many teams to assign
  bne 0x8 
  li r6, totalTeams
  mr r30, r6  

  #initialise list filled with one random team 
  %randi(totalTeams) 
  li r4, 0x8
initialLoop:   #set everyone to base team 
  stwx r3, r1, r4 
  addi r4, r4, 0x4
  cmpwi r4, 0x14
  ble initialLoop

  %randi(totalTeamsSubOne)
  addi r29, r3, 1   #teamTwoOffset, loop through teams available by doing team = (team[0] + teamsTwoOffset*teamNum) % totalTeams
  li r28, 0
assignTeams:
  addi r28, r28, 1 #start at team 1 since team 0 is filled out already
  cmpw r28, r30
  bge end
  li r3, -1
  cmpwi r30, 2
  bne calcTeam
  %randi(3) #selects a random port from remaining ones to ignore
calcTeam:
  lwz r4, 0x8(r1) #teamOne
  mullw r5, r28, r29
  add r4, r5, r4
moduloTeams:
  cmpwi r4, totalTeams
  blt moduloDone
  subi r4, r4, totalTeams
  b moduloTeams
moduloDone:
  #r3 = gap
  #r4 = team
  #r5 = where to store into array
  mulli r5, r28, 4
  addi r5, r5, 0x4
assignTeamLoop:
  addi r5, r5, 4
  cmpwi r5, 0x14
  bgt assignTeams
  cmpwi r3, 0
  subi r3, r3, 1
  beq assignTeamLoop
  stwx r4, r1, r5
  b assignTeamLoop
end:
  lis r12, 0x8068
  ori r12, r12, 0xAD1C
  mtctr r12
  bctr   
}