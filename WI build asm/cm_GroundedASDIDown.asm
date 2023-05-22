Enable ASDI Downwards while Grounded v1.1 [Magus]
# Code Menu Variant by Eon, mawwwk
# Remove corresponding code in Source\ProjectM\DirectionalInfluence.asm
.alias CodeMenuStart = 0x804E
.alias CodeMenuHeader = 0x02D8     #Offset of word containing location of code menu toggle

# Original code PM:
# op b 0x4C @ $80876F88
# op b 0x4C @ $80876698
# Original Brawl instr:
# bne- 0x4C @ ...

HOOK @ $80876F88
{
   bne ASDIdown_a               # vBrawl compare
   lis r3, CodeMenuStart
   ori r3, r3, CodeMenuHeader   # \ Load Code Menu Header
   lwz r3, 0(r3)
   lbz r3, 0xB(r3)
   cmpwi r3, 1                  # / Skip if toggle ON (PM compare)
   bne %end%
ASDIDown_a:
   lis r12, 0x8087
   ori r12, r12, 0x6FD4 # 0x6F88 + 0x4C
   mtctr r12
   bctr
}

HOOK @ $80876698
{
  bne ASDIDown_b                # vBrawl compare
  lis r3, CodeMenuStart
  ori r3, r3, CodeMenuHeader    # \ Load Code Menu Header
  lwz r3, 0(r3)
  lbz r3, 0xB(r3)
  cmpwi r3, 1                   # / Skip if toggle ON (PM compare)
  bne %end%
ASDIDown_b:
  lis r12, 0x8087
  ori r12, r12, 0x66E4 # 0x6698 + 0x4C
  mtctr r12
  bctr
}
