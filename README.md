# Wisconsin-pplus-codemenu

Fork of Fracture's code menu as used in Project+ 2.2.

# Changes:
- Various options re-arranged for organization and user-friendliness
- Salty Runback button combination includes A+B, in addition to L+R+Y
- Unchanged lines are now a slight grey color, with the currently-selected line remaining yellow regardless of value change
- Gameplay Modifiers and Flight Mode are re-sorted into their own sub-menus with properly capped values
- Start-alts replace Y-alts in random Alternate Stages
- Includes Eon's fixes from P+ 2.27 onward for debug overlay flickers, and other stability fixes

Added options:
- Stagelist (by Desi, with custom 5-stagelist implementation)
- Balloon Hit Behavior (base code thanks to Eon)
- Big Head Mode (by Eon, format by Desi)
- Grab Trade Behavior - Default, Recoil (clank), Heart Swap (Manaphy Pokeball effect). by Eon
- Player Codes: Flash on Missed L-Cancel (by PMDT, modified from Desi's variant)
- Gameplay Modifier: Random Knockback Angle (by Eon, format by Desi)
- Gameplay Modifier: Universal Walljumping (address + parameters thanks to Eon)
- Gameplay Modifier: Hitfalling - fastfall aerials on-hit (by Eon)
- Gameplay Modifier: Grounded ASDI Down toggle (by Eon, Magus)
- Gameplay Modifier: Crouch Knockback Multiplier (address thanks to Magus)
- Gameplay Modifier: Shield Decay and Regen Rate (addresses thanks to Snoe)


#### Credits and thanks to the legends that made this fork possible at all:  
- **Fracture** for the base code menu architecture as it appears in Legacy TE and Project+, as well as all the great tech that comes with it.  
- **DesiacX** for the helpful guides and implementations of adding external toggle codes, especially the Stagelist toggle configuration.  
- **Eon** for providing the 2.27 debug overlay fix code, and for creating various codes used here.  
- **DukeItOut** for designing the GCTRM format which streamlines the gecko coding process start-to-finish, along with the flexible STEX file system which makes the Stagelist toggle possible.
