# Wisconsin Project+ code menu

Fork of Fracture's code menu as used in Project+ 2.2, with the latest release for use with Project+ v2.4.1.

Intended only for use with WI P+ build v1.5, but forks are welcome. Please give credit and open-source any edits for custom builds!

# Changes:
- Recolored text & background colors with less contrast to be easier on the eyes
- Includes shield color & theme toggles built-in, among other new toggles. Fuctionality for many toggles requires external asm files included in this repo
- Salty Runback button combination loosened to R+Y. Skip Results combo loosened to R+X
- Start-alts replace Y-alts in random Alternate Stages
- Includes Eon's fixes from P+ 2.27 onward for debug overlay flickers, and other stability fixes
- Player Codes reorganized into a separate sub-menu, sorted by feature instead of player port
  - Infinite Shields consolidated into a single toggle
  - Set Percent toggle re-sorted into a single sub-menu, with one toggle for all ports 
- Various other options re-arranged for organization and readability
  - Debug Settings reworded with new comments & easier to read toggle phrasing
  - Gameplay Modifiers & Flight Mode are re-sorted into their own sub-menus with properly capped values

Added options:
- Stagelist: Set stage slots on page 1 and update random select accordingly
- Theme: Load alternate selchar, selchar2, and selmap files
- Timeout Ledgegrab Limit: If a game goes to time, and any player reaches a ledgegrab threshold, determine winner by lower ledgegrab count
- (Netplay only) Ordered stage choice: Override stage choice setting for an even stage distribution in netplay sessions
- Balloon Hit Behavior: Add or subtract a stock upon hitting the Smashville balloon (by Eon)
- Big Head Mode (by Eon)
- Grab Trade Behavior: Default, recoil (grab clank), or Heart Swap (player controls switch). (by Eon)
- Screen Shake toggle: Disable camera shake in certain situations (by Eon, custom implementation)
- 1P Countdown: Re-enable countdown & entry animations for 1-player matches (code by JOJI, DukeItOut)
- Player Codes - Flash on Missed L-Cancel (custom implementation, original toggle by Desi, PMDT)
- Player Codes - Shield Color: Choose from nine available colors; five default + four custom hues
- Gameplay Modifier - Random Knockback Angle (by Eon, implementation by DesiacX)
- Gameplay Modifier - Universal Walljumping - allow all characters to walljump (address + parameters thanks to Eon)
- Gameplay Modifier - Hitfalling - fastfall aerials on-hit (by Eon, custom implementation)
- Gameplay Modifier - Grounded ASDI Down toggle (by Eon, Magus)
- Gameplay Modifier - Crouch Knockback Multiplier (address thanks to Magus)
- Gameplay Modifier - Shield Decay and Regen Rate (addresses thanks to Snoe)
- Gameplay Modifier - DI Amplitude (address thanks to Eon)

#### Credits and thanks to the modders that made these edits possible:  
- **Fracture** for the base code menu architecture as it appears in Legacy TE and Project+, as well as all the great tech that came with it.  
- **DesiacX** for the helpful guides and implementations of adding external code toggles
- **Eon** for providing the v2.27 debug overlay fix code, and for creating various codes used here.  
- **DukeItOut** for designing the GCTRM format which streamlines the Gecko coding process at every step, along with the flexible STEX file system which makes the Stagelist toggle possible, among others.
