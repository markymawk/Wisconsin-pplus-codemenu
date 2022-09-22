######################################################################################
Set menus based on Theme code menu setting [Bird, mawwwk]
######################################################################################
# Theme toggle location: 0x804E0793 

* 264E0793 00000001 # If value is less than 1
string "/menu2/sc_selcharacter.pac"     @ $806FF2EC
string "sc_selcharacter_en.pac"         @ $817F6365
string "/menu2/sc_selcharacter2.pac"    @ $806FF308
string "sc_selcharacter2_en.pac"        @ $817F634D
* E0000000 80008000

* 204E0793 00000001 # If value is equal to 1
string "/menu2/cx_selcharacter.pac"     @ $806FF2EC
string "cx_selcharacter_en.pac"         @ $817F6365
string "/menu2/cx_selcharacter2.pac"    @ $806FF308
string "cx_selcharacter2_en.pac"        @ $817F634D
* E0000000 80008000

* 204E0793 00000002 # If value is equal to 2
string "/menu2/in_selcharacter.pac"     @ $806FF2EC
string "in_selcharacter_en.pac"         @ $817F6365
string "/menu2/in_selcharacter2.pac"    @ $806FF308
string "in_selcharacter2_en.pac"        @ $817F634D
* E0000000 80008000