#include "stdafx.h"
#include "ReplayFix.h"
#include "Controls.h"
#include "StopStartAlts.h"
#include "Save States.h"
#include "Miscellaneous Code.h"
#include "StopDPadOnSSS (except Wiimote).h"
#include "DrawDI.h"
#include "Code Menu.h"
#include "IASA Overlay.h"
#include "Control Codes.h"
#include "Last Character Auto Select.h"
#include "Tag Based Costumes.h"
#include "Light Shield.h"
#include "IkeClimbers.h"
#include "AIDisplay.h"
#include "C++Injection.h"
//#include "FPS Display.h"
using namespace std;



int main()
{
	#if NETPLAY_BUILD
	string OutputTextPath = "G:\\Files\\Wisco PM\\Code menu repos\\asm-net.txt";
	#else
	string OutputTextPath = "G:\\Files\\Wisco PM\\Code menu repos\\asm.txt";
	#endif

	CodeStart(OutputTextPath);

	CodeMenu(); tagBasedCostumes();

	CodeEnd();

	return 0;
}