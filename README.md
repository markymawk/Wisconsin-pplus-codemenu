# Wisconsin Project+ code menu

Fork of Fracture's code menu as used in Project+ 2.2.

# Changes:
- Unchanged lines are a slight grey color, with the currently-selected line remaining yellow regardless of value change
- Salty Runback button combination includes A+B, in addition to L+R+Y. Skip Results combo loosened to R+X
- Start-alts replace Y-alts in random Alternate Stages
- Includes Eon's fixes from P+ 2.27 onward for debug overlay flickers, and other stability fixes
- Gameplay Modifiers &  Flight Mode are re-sorted into their own sub-menus with properly capped values
- Debug Settings reworded with new comments & easier to read toggle phrasing
- Various other options re-arranged for organization, readability and all that

Added options:
- Stagelist: Set stage slots on page 1 and update random select accordingly (by Desi, with custom implementation)
- Theme: load alternate selchar, selchar2, selmap files (thanks to Bird for this concept)
- Balloon Hit Behavior (by Eon)
- Big Head Mode (by Eon, format by Desi)
- Grab Trade Behavior - Default, Recoil (clank), Heart Swap (Manaphy Pokeball effect). by Eon
- Player Codes: Flash on Missed L-Cancel (modified from Desi's variant, original Flash code by PMDT)
- Gameplay Modifier: Random Knockback Angle (by Eon, format by Desi)
- Gameplay Modifier: Universal Walljumping - allow all characters to walljump (address + parameters thanks to Eon)
- Gameplay Modifier: Hitfalling - fastfall aerials on-hit (by Eon)
- Gameplay Modifier: Grounded ASDI Down toggle (by Eon, Magus)
- Gameplay Modifier: Crouch Knockback Multiplier (address thanks to Magus)
- Gameplay Modifier: Shield Decay and Regen Rate (addresses thanks to Snoe)
- Gameplay Modifier: DI Amplitude (address thanks to Eon)


#### Credits and thanks to the legends that made this fork possible at all:  
- **Fracture** for the base code menu architecture as it appears in Legacy TE and Project+, as well as all the great tech that comes with it.  
- **DesiacX** for the helpful guides and implementations of adding external toggle codes, especially the Stagelist toggle configuration.  
- **Eon** for providing the 2.27 debug overlay fix code, and for creating various codes used here.  
- **DukeItOut** for designing the GCTRM format which streamlines the gecko coding process start-to-finish, along with the flexible STEX file system which makes the Stagelist toggle possible.
