#include "stdafx.h"
#include "Code Menu.h"
#include "DrawDI.h"
#include "IASA Overlay.h"
#include "Control Codes.h"
#include "Save States.h"
#include "DBZ Mode.h"
#include <regex>

int MENU_TITLE_CHECK_LOCATION;
int DI_DRAW_INDEX = -1;
int DEBUG_MODE_INDEX = -1;
int DISPLAY_HITBOXES_INDEX = -1;
int DISPLAY_COLLISION_INDEX = -1;
int DISPLAY_LEDGEGRAB_INDEX = -1;
int HUD_DISPLAY_INDEX = -1;
int CHARACTER_SELECT_P1_INDEX = -1;
int CHARACTER_SELECT_P2_INDEX = -1;
int CHARACTER_SELECT_P3_INDEX = -1;
int CHARACTER_SELECT_P4_INDEX = -1;
int INFINITE_SHIELDS_P1_INDEX = -1;
int INFINITE_SHIELDS_P2_INDEX = -1;
int INFINITE_SHIELDS_P3_INDEX = -1;
int INFINITE_SHIELDS_P4_INDEX = -1;
int CAMERA_LOCK_INDEX = -1;
int ENDLESS_FRIENDLIES_MODE_INDEX = -1;
int ENDLESS_FRIENDLIES_STAGE_SELECTION_INDEX = -1;
int RANDOM_1_TO_1_INDEX = -1;
int AUTO_SAVE_REPLAY_INDEX = -1;
int SAVE_STATES_INDEX = -1;
int SAVE_REPLAY_ANYWHERE_INDEX = -1;
int AUTO_SKIP_TO_CSS_INDEX = -1;
int CODE_MENU_ACTIVATION_SETTING_INDEX = -1;
int PERCENT_SELECT_VALUE_P1_INDEX = -1;
int PERCENT_SELECT_ACTIVATOR_P1_INDEX = -1;
int PERCENT_SELECT_VALUE_P2_INDEX = -1;
int PERCENT_SELECT_ACTIVATOR_P2_INDEX = -1;
int PERCENT_SELECT_VALUE_P3_INDEX = -1;
int PERCENT_SELECT_ACTIVATOR_P3_INDEX = -1;
int PERCENT_SELECT_VALUE_P4_INDEX = -1;
int PERCENT_SELECT_ACTIVATOR_P4_INDEX = -1;
int DISABLE_DPAD_P1_INDEX = -1;
int DISABLE_DPAD_P2_INDEX = -1;
int DISABLE_DPAD_P3_INDEX = -1;
int DISABLE_DPAD_P4_INDEX = -1;
int FPS_DISPLAY_INDEX = -1;
int DBZ_MODE_INDEX = -1;
int DBZ_MODE_MAX_SPEED_X_INDEX = -1;
int DBZ_MODE_ACCEL_X_INDEX = -1;
int DBZ_MODE_MAX_SPEED_Y_INDEX = -1;
int DBZ_MODE_ACCEL_Y_INDEX = -1;
int ALT_STAGE_BEHAVIOR_INDEX = -1;
int P1_TAG_STRING_INDEX = -1;
int P2_TAG_STRING_INDEX = -1;
int P3_TAG_STRING_INDEX = -1;
int P4_TAG_STRING_INDEX = -1;
int TAG_COSTUME_TOGGLE_INDEX = -1;
int CROWD_CHEER_TOGGLE_INDEX = -1;
int STALING_TOGGLE_INDEX = -1;
int ALC_P1_INDEX = -1;
int ALC_P2_INDEX = -1;
int ALC_P3_INDEX = -1;
int ALC_P4_INDEX = -1;
int ALC_P1_FLASH_RED_INDEX = -1;
int ALC_P2_FLASH_RED_INDEX = -1;
int ALC_P3_FLASH_RED_INDEX = -1;
int ALC_P4_FLASH_RED_INDEX = -1;
int BIG_HEAD_INDEX = -1;
int RANDOM_ANGLE_INDEX = -1;
int WAR_MODE_INDEX = -1;
int BUFFER_P1_INDEX = -1;
int BUFFER_P2_INDEX = -1;
int BUFFER_P3_INDEX = -1;
int BUFFER_P4_INDEX = -1;
int SCALE_INDEX = -1;
int SPEED_INDEX = -1;
int CSS_VERSION_SETTING_INDEX = -1;
int THEME_SETTING_INDEX = -1;
int DASH_ATTACK_ITEM_GRAB_INDEX = -1;
int TRIP_TOGGLE_INDEX = -1;
int TRIP_RATE_MULTIPLIER_INDEX = -1;
int TRIP_INTERVAL_INDEX = -1;
int BACKPLATE_COLOR_1_INDEX = -1;
int BACKPLATE_COLOR_2_INDEX = -1;
int BACKPLATE_COLOR_3_INDEX = -1;
int BACKPLATE_COLOR_4_INDEX = -1;
int BACKPLATE_COLOR_C_INDEX = -1;
int BACKPLATE_COLOR_T_INDEX = -1;
int JUMPSQUAT_OVERRIDE_TOGGLE_INDEX = -1;
int JUMPSQUAT_OVERRIDE_FRAMES_INDEX = -1;
int JUMPSQUAT_OVERRIDE_MIN_INDEX = -1;
int JUMPSQUAT_OVERRIDE_MAX_INDEX = -1;
int EXTERNAL_INDEX = -1;	//Used for GCTRM codes that use other indexs for context

//constant overrides
vector<ConstantPair> constantOverrides;
int SDI_DISTANCE_INDEX = -1;
int HITSTUN_MULTIPLIER_INDEX = -1;
int HITLAG_MAXIMUM_INDEX = -1;
int HITLAG_MULTIPLIER_INDEX = -1;
int ELECTRIC_HITLAG_MULTIPLIER_INDEX = -1;
int ASDI_DISTANCE_INDEX = -1;
int WALLJUMP_HORIZONTAL_MULTIPLIER_INDEX = -1;
int MINIMUM_SHIELD_SIZE_SCALING_INDEX = -1;
int SHIELD_DAMAGE_MULTIPLIER_INDEX = -1;
int SHIELD_BASE_DAMAGE_INDEX = -1;
int SHIELD_SIZE_MULTIPLIER_INDEX = -1;
int SHIELD_TILT_MULTIPLIER_INDEX = -1;
int KNOCKBACK_DECAY_MULTIPLIER_INDEX = -1;
int WALL_BOUNCE_KNOCKBACK_MULTIPLIER_INDEX = -1;


int SHIELD_RED_1 = -1;
int SHIELD_GREEN_1 = -1;
int SHIELD_BLUE_1 = -1;
int SHIELD_ALPHA_1 = -1;
int SHIELD_RED_2 = -1;
int SHIELD_GREEN_2 = -1;
int SHIELD_BLUE_2 = -1;
int SHIELD_ALPHA_2 = -1;
int SHIELD_RED_3 = -1;
int SHIELD_GREEN_3 = -1;
int SHIELD_BLUE_3 = -1;
int SHIELD_ALPHA_3 = -1;
int SHIELD_RED_4 = -1;
int SHIELD_GREEN_4 = -1;
int SHIELD_BLUE_4 = -1;
int SHIELD_ALPHA_4 = -1;

int tets = 0x935fe30C;


vector<string> CHARACTER_LIST{};
vector<u16> CHARACTER_ID_LIST{};
void buildCharacterIDLists()
{
	std::map<std::string, u16> characterNameToIDMap =
	{
		{"Bowser", LCSI_BOWSER},
		{"Captain Falcon", LCSI_CAPTAIN_FALCON},
		{"Charizard", LCSI_CHARIZARD},
		{"Dedede", LCSI_DEDEDE},
		{"Diddy Kong", LCSI_DIDDY_KONG},
		{"Donkey Kong", LCSI_DONKEY_KONG},
		{"Falco", LCSI_FALCO},
		{"Fox", LCSI_FOX},
		{"Ganondorf", LCSI_GANONDORF},
		{"Ice Climbers", LCSI_ICE_CLIMBERS},
		{"Ike", LCSI_IKE},
		{"Ivysaur", LCSI_IVYSAUR},
		{"Jigglypuff", LCSI_JIGGLYPUFF},
		{"Kirby", LCSI_KIRBY},
		{"Link", LCSI_LINK},
		{"Lucario", LCSI_LUCARIO},
		{"Lucas", LCSI_LUCAS},
		{"Luigi", LCSI_LUIGI},
		{"Mario", LCSI_MARIO},
		{"Marth", LCSI_MARTH},
		{"Meta Knight", LCSI_META_KNIGHT},
		{"Mr. Game and Watch", LCSI_MR_GAME_AND_WATCH},
		{"Ness", LCSI_NESS},
		{"Olimar", LCSI_OLIMAR},
		{"Peach", LCSI_PEACH},
		{"Pikachu", LCSI_PIKACHU},
		{"Pit", LCSI_PIT},
		{"R.O.B.", LCSI_ROB},
		{"Samus", LCSI_SAMUS},
		{"Sheik", LCSI_SHEIK},
		{"Snake", LCSI_SNAKE},
		{"Sonic", LCSI_SONIC},
		{"Squirtle", LCSI_SQUIRTLE},
		{"Toon Link", LCSI_TOON_LINK},
		{"Wario", LCSI_WARIO},
		{"Wolf", LCSI_WOLF},
		{"Yoshi", LCSI_YOSHI},
		{"Zelda", LCSI_ZELDA},
		{"Zero Suit Samus", LCSI_ZERO_SUIT_SAMUS }
	};
	if (characterListVersion >= characterListVersions::clv_vBRAWL_SPECIAL_CHARS)
	{
		characterNameToIDMap.emplace("Giga Bowser", LCSI_GIGA_BOWSER);
		characterNameToIDMap.emplace("Sopo", LCSI_SOPO);
		characterNameToIDMap.emplace("Warioman", LCSI_WARIOMAN);
	}
	if (characterListVersion >= characterListVersions::clv_PROJECTM)
	{
		characterNameToIDMap.emplace("Mewtwo", LCSI_MEWTWO);
		characterNameToIDMap.emplace("Roy", LCSI_ROY);
	}
	if (characterListVersion >= characterListVersions::clv_PPLUS)
	{
		characterNameToIDMap.emplace("Knuckles", LCSI_KNUCKLES);
	}
	if (characterListVersion >= characterListVersions::clv_PPEX_RIDLEY)
	{
		characterNameToIDMap.emplace("Ridley", LCSI_RIDLEY);
	}
	if (characterListVersion >= characterListVersions::clv_PPEX_WALUIGI)
	{
		characterNameToIDMap.emplace("Waluigi", LCSI_WALUIGI);
	}
	if (characterListVersion >= characterListVersions::clv_PPEX_ALLOYS)
	{
		characterNameToIDMap.emplace("Red Alloy", LCSI_ALLOY_RED);
		characterNameToIDMap.emplace("Blue Alloy", LCSI_ALLOY_BLUE);
		characterNameToIDMap.emplace("Yellow Alloy", LCSI_ALLOY_YELLOW);
		characterNameToIDMap.emplace("Green Alloy", LCSI_ALLOY_GREEN);
	}
	if (characterListVersion >= characterListVersions::clv_PPEX_DARK_SAMUS)
	{
		characterNameToIDMap.emplace("Dark Samus", LCSI_DARK_SAMUS);
	}
	if (characterListVersion >= characterListVersions::clv_PPEX_SCEPTILE)
	{
		characterNameToIDMap.emplace("Sceptile", LCSI_SCEPTILE);
	}

	unzipMapToVectors(characterNameToIDMap, CHARACTER_LIST, CHARACTER_ID_LIST);
}
bool applyCharacterListVersion(unsigned long targetVersion)
{
	bool result = 0;

	// If the targeted version is valid...
	if (targetVersion < characterListVersions::__clv_Count)
	{
		// ... replace the recorded list version...
		characterListVersion = targetVersion;
		// ... and rebuild the character lists.
		buildCharacterIDLists();
		result = 1;
	}

	return result;
}


vector<string> ROSTER_LIST{};
vector<string> ROSTER_FILENAME_LIST{};
void buildRosterLists()
{
	std::map<std::string, std::string> rosterNameToFileIDMap =
	{
		{"Default", "CSSRoster.dat"}
	};

	unzipMapToVectors(rosterNameToFileIDMap, ROSTER_LIST, ROSTER_FILENAME_LIST);
}

namespace themeConstants
{
	std::array<std::string, tpi__PATH_COUNT> filenames{};
}
void initializeThemeConstants()
{
	themeConstants::filenames.fill("");
	themeConstants::filenames[themeConstants::tpi_MENUMAIN] = "mu_menumain.pac";
	themeConstants::filenames[themeConstants::tpi_SELCHAR] = "sc_selcharacter.pac";
	themeConstants::filenames[themeConstants::tpi_SELCHAR2] = "sc_selcharacter2.pac";
	themeConstants::filenames[themeConstants::tpi_SELMAP] = "sc_selmap.pac";
	themeConstants::filenames[themeConstants::tpi_SELEVENT] = "sc_sel_event.pac";
	themeConstants::filenames[themeConstants::tpi_TITLE] = "sc_title.pac";
}
menuTheme::menuTheme(std::string name)
{
	prefixes[themeConstants::tpi_MENUMAIN] = getThemeFileDefaultPrefix(themeConstants::tpi_MENUMAIN);
	prefixes[themeConstants::tpi_SELCHAR] = getThemeFileDefaultPrefix(themeConstants::tpi_SELCHAR);
	prefixes[themeConstants::tpi_SELCHAR2] = getThemeFileDefaultPrefix(themeConstants::tpi_SELCHAR2);
	prefixes[themeConstants::tpi_SELMAP] = getThemeFileDefaultPrefix(themeConstants::tpi_SELMAP);
	prefixes[themeConstants::tpi_SELEVENT] = getThemeFileDefaultPrefix(themeConstants::tpi_SELEVENT);
	prefixes[themeConstants::tpi_TITLE] = getThemeFileDefaultPrefix(themeConstants::tpi_TITLE);
}
void buildThemeLists()
{
	initializeThemeConstants();
	std::map<std::string, menuTheme> themeNameToThemeSpecMap =
	{
		{"Default", menuTheme("Default")},
	};
	unzipMapToVectors(themeNameToThemeSpecMap, THEME_LIST, THEME_SPEC_LIST);
}
std::string getThemeFileBaseName(themeConstants::themePathIndices fileIndex)
{
	return themeConstants::filenames[fileIndex].substr(themeConstants::prefixLength, std::string::npos);
}
std::string getThemeFileDefaultPrefix(themeConstants::themePathIndices fileIndex)
{
	return themeConstants::filenames[fileIndex].substr(0, themeConstants::prefixLength);
}
std::vector<string> THEME_LIST;
std::vector<menuTheme> THEME_SPEC_LIST{};
std::array<bool, themeConstants::tpi__PATH_COUNT> THEME_FILE_GOT_UNIQUE_PREFIX{};

namespace backplateColorConstants
{
	const std::array<std::string, playerSlotColorLevel::pSCL__COUNT> modeNames =
	{
		"Disabled",
		"Shields & Death Plumes",
		"Shields, Death Plumes, & In-Game HUD",
		"All In-Game Elements, CSS, & Results Screen",
		"All Elements & CSS Color Change Input",
	};
}
const unsigned long BACKPLATE_COLOR_TOTAL_COLOR_COUNT = 10;

// Incoming Configuration XML Variables
std::vector<std::string> CONFIG_INCOMING_COMMENTS{};
bool CONFIG_DELETE_CONTROLS_COMMENTS = false;
unsigned char CONFIG_BACKPLATE_COLOR_MODE = backplateColorConstants::pSCL_NONE;
bool CONFIG_DASH_ATTACK_ITEM_GRAB_ENABLED = 1;
bool CONFIG_JUMPSQUAT_OVERRIDE_ENABLED = 1;


#if PROJECT_PLUS_EX_BUILD
const std::string menuConfigXMLFileName = "EX_Config.xml";
#else
const std::string menuConfigXMLFileName = "Config.xml";
#endif
const std::string netMenuConfigXMLFileName = "Net-" + menuConfigXMLFileName;
const std::string outputFolder = "./Code_Menu_Output/";
const std::string symbolMapInputFileName = "symbols.map";
const std::string buildFolder = ".././";
const std::string GCTRMExePath = buildFolder + "GCTRealMate.exe";
const std::string GCTRMCommandBase = "\"" + GCTRMExePath + "\" -g -l -q ";

// !!Important Note on cmnuFileName and cmnuBuildLocationDirectory!!
// These variables are also referenced in "Control Codes.cpp"'s "LoadCodeMenu" function,
// in which they're directly used to determine where the game will look for the Code Menu.
// For that reason, please make sure you properly define those variables, as failing
// to do so will cause general weirdness (eg. invisible shields, skipping results screen, etc). 
#if BUILD_TYPE == PROJECT_PLUS
// Constant Confiuguration Block for Project+ Based Builds
	#if BUILD_NETPLAY_FILES == true
	const std::string changelogFileName = "Net-Code_Menu_Changelog.txt";
	const std::string optionsFilename = "Net-Code_Menu_Options.xml";
	const std::string mainGCTName = "NETPLAY";
	const std::string boostGCTName = "NETBOOST";
	const std::string asmFileName = "Net-CodeMenu.asm";
	const std::string asmTextFileName = "Net-CodeMenu.txt";
	const std::string cmnuFileName = "dnet.cmnu";
	const std::string asmBuildLocationDirectory = "Source/Netplay/";
	const std::string cmnuBuildLocationDirectory = "pf/menu3/";
	#elif BUILD_NETPLAY_FILES == false
	const std::string changelogFileName = "Code_Menu_Changelog.txt";
	const std::string optionsFilename = "Code_Menu_Options.xml";
	const std::string mainGCTName = "RSBE01";
	const std::string boostGCTName = "BOOST";
	const std::string asmFileName = "CodeMenu.asm";
	const std::string asmTextFileName = "CodeMenu.txt";
	const std::string cmnuFileName = "data.cmnu";
	const std::string asmBuildLocationDirectory = "Source/Project+/";
	const std::string cmnuBuildLocationDirectory = "pf/menu3/";
	#endif
#else
// Default Constant Confiuguration Block (used currently by PMEX and LegacyXP)
// Note: These builds don't use the more modern Source Folder based config, just RSBE01.gct
//		As a result, the following constants are left blank on purpose, as they aren't used:
//		- boostGCTName
//		- asmFileName
//		- asmTextFileName
//		- asmBuildLocationDirectory
const std::string changelogFileName = "Code_Menu_Changelog.txt";
const std::string optionsFilename = "Code_Menu_Options.xml";
const std::string mainGCTName = "RSBE01";
const std::string boostGCTName = "";
const std::string asmFileName = "";
const std::string asmTextFileName = "";
const std::string cmnuFileName = "cm.bin";
const std::string asmBuildLocationDirectory = "";
const std::string cmnuBuildLocationDirectory = "./";
#endif

const std::string mainGCTFile = buildFolder + mainGCTName + ".GCT";
const std::string mainGCTTextFile = buildFolder + mainGCTName + ".txt";
const std::string boostGCTFile = buildFolder + boostGCTName + ".GCT";
const std::string boostGCTTextFile = buildFolder + boostGCTName + ".txt";
const std::string asmOutputFilePath = outputFolder + asmFileName;
const std::string asmTextOutputFilePath = outputFolder + asmTextFileName;
const std::string cmnuOutputFilePath = outputFolder + cmnuFileName;
const std::string cmnuOptionsOutputFilePath = outputFolder + optionsFilename;
const std::string asmBuildLocationFilePath = buildFolder + asmBuildLocationDirectory + asmFileName;
const std::string cmnuBuildLocationFilePath = buildFolder + cmnuBuildLocationDirectory + cmnuFileName;
std::string getCMNUAbsolutePath()
{
	return MAIN_FOLDER + "/" + cmnuBuildLocationDirectory + cmnuFileName;
}

void initMenuFileStream()
{
	MenuFile.open(cmnuOutputFilePath, fstream::out | fstream::binary);
}



// Options File Functions
namespace xmlTagConstants
{
	const std::string codeMenuTag = "codeMenu";
	const std::string nameTag = "name";
	const std::string indexTag = "index";
	const std::string valueTag = "value";
	const std::string valueMinTag = "minValue";
	const std::string valueMaxTag = "maxValue";
	const std::string valueDefaultTag = "defaultValue";
	const std::string editableTag = "editable";
	const std::string cmnuPathTag = "cmnuPath";
	const std::string pageTag = "codeMenuPage";
	const std::string selectionTag = "codeMenuSelection";
	const std::string selectionDefaultTag = "defaultOption";
	const std::string selectionOptionTag = "option";
	const std::string intTag = "codeMenuInt";
	const std::string floatTag = "codeMenuFloat";
	const std::string lockedTag = "locked";
}
pugi::xml_document menuOptionsTree{};
bool loadMenuOptionsTree(std::string xmlPathIn, pugi::xml_document& destinationDocument)
{
	bool result = 0;

	if (std::filesystem::is_regular_file(xmlPathIn))
	{
		if (destinationDocument.load_file(xmlPathIn.c_str()))
		{
			result = 1;
		}
	}

	return result;
}
void recursivelyFindPages(Page& currBasePageIn, std::vector<Page*>& collectedPointers)
{
	for (unsigned long i = 0; i < currBasePageIn.Lines.size(); i++)
	{
		const Line* currLine = currBasePageIn.Lines[i];
		if (currLine->type == SUB_MENU_LINE)
		{
			bool pointerNotPreviouslyCollected = 1;
			for (unsigned long u = 0; pointerNotPreviouslyCollected && u < collectedPointers.size(); u++)
			{
				pointerNotPreviouslyCollected &= collectedPointers[u] != currLine->SubMenuPtr;
			}
			if (pointerNotPreviouslyCollected)
			{
				collectedPointers.push_back(currLine->SubMenuPtr);
				recursivelyFindPages(*currLine->SubMenuPtr, collectedPointers);
			}
		}
	}
}
void findPagesInOptionsTree(const pugi::xml_document& optionsTree, std::map<std::string, pugi::xml_node>& collectedNodes)
{
	// Request the code menu base node from the optionsTree...
	pugi::xml_node menuNode = optionsTree.child(xmlTagConstants::codeMenuTag.c_str());
	// ... and if it was validly returned...
	if (menuNode)
	{
		// ... get the collection of page nodes from the menu.
		pugi::xml_object_range pageNodes = menuNode.children(xmlTagConstants::pageTag.c_str());
		// For each of these pages...
		for (pugi::xml_named_node_iterator pageItr = pageNodes.begin(); pageItr != pageNodes.end(); pageItr++)
		{
			// ... request its name attribute...
			pugi::xml_attribute pageNameAttr = pageItr->attribute(xmlTagConstants::nameTag.c_str());
			// ... and if it's validly returned...
			if (pageNameAttr)
			{
				// ... record the page and its name in our map.
				collectedNodes[pageNameAttr.value()] = *pageItr;
			}
		}
	}
}
void findLinesInPageNode(const pugi::xml_node& pageNode, std::map<std::string, pugi::xml_node>& collectedNodes)
{
	for (pugi::xml_node_iterator lineItr = pageNode.begin(); lineItr != pageNode.end(); lineItr++)
	{
		if (lineItr->name() == xmlTagConstants::selectionTag || lineItr->name() == xmlTagConstants::floatTag || lineItr->name() == xmlTagConstants::intTag)
		{
			// Request the name attribute from the current node...
			pugi::xml_attribute nameAttr = lineItr->attribute(xmlTagConstants::nameTag.c_str());
			// ... and if the returned attribute is valid...
			if (nameAttr)
			{
				// ... record it and the corresponding line in the output map.
				collectedNodes[nameAttr.value()] = *lineItr;
			}
		}
	}
}
std::vector<const char*> splitLineContentString(const std::string& joinedStringIn)
{
	std::vector<const char*> result{};

	unsigned long currStringIndex = 0x00;
	for (unsigned long i = 0; i < joinedStringIn.size(); i++)
	{
		if (joinedStringIn[i] == 0x00)
		{
			result.push_back(joinedStringIn.data() + currStringIndex);
			currStringIndex = i + 1;
		}
	}
	if (currStringIndex < joinedStringIn.size())
	{
		result.push_back(joinedStringIn.data() + currStringIndex);
	}

	return result;
}
void applyLineSettingsFromMenuOptionsTree(Page& mainPageIn, const pugi::xml_document& xmlDocumentIn)
{
	// Get a list of all the pages in the menu, including the main page.
	std::vector<Page*> Pages{ &mainPageIn };
	recursivelyFindPages(mainPageIn, Pages);

	// And find every page present in the XML.
	std::map<std::string, pugi::xml_node> pageNodeMap;
	findPagesInOptionsTree(xmlDocumentIn, pageNodeMap);

	// For each of the pages we found in our actual menu structure...
	for (Page* currPage : Pages)
	{
		// ... see if there was a corresponding page in the XML document...
		auto pageFindItr = pageNodeMap.find(currPage->PageName);
		if (pageFindItr == pageNodeMap.end()) continue;

		

		// If there was, we need to apply the default values from the lines in that page node!
		// Additionally, get a list of all the line nodes in this page node.
		std::map<std::string, pugi::xml_node> lineNodeMap;
		findLinesInPageNode(pageFindItr->second, lineNodeMap);
		// Then, for each line in the current page struct...
		for (Line* currLine : currPage->Lines)
		{
			// ... check if a corresponding node is present in this page node.
			std::vector<const char*> deconstructedText = splitLineContentString(currLine->Text);
			auto lineFindItr = lineNodeMap.find(deconstructedText[0]);
			if (lineFindItr == lineNodeMap.end()) continue;

			// If there was, pull the default value recorded in the XML and write it into the actual line struct (based on the line type).
			switch (currLine->type)
			{
				case SELECTION_LINE:
				{
					pugi::xml_node defaultValNode = lineFindItr->second.child(xmlTagConstants::selectionDefaultTag.c_str());
					if (defaultValNode)
					{
						pugi::xml_attribute defaultIndexAttr = defaultValNode.attribute(xmlTagConstants::indexTag.c_str());
						if (defaultIndexAttr)
						{
							u32 valueIn = defaultIndexAttr.as_uint(currLine->Default);
							valueIn = std::min<unsigned long>(std::max(0u, valueIn), deconstructedText.size() - 2);
							currLine->Default = valueIn;
							currLine->Value = valueIn;
						}
					}
					break;
				}
				case INTEGER_LINE:
				{
					pugi::xml_node defaultValNode = lineFindItr->second.child(xmlTagConstants::valueDefaultTag.c_str());
					if (defaultValNode)
					{
						pugi::xml_attribute defaultValueAttr = defaultValNode.attribute(xmlTagConstants::valueTag.c_str());
						if (defaultValueAttr)
						{
							int valueIn = defaultValueAttr.as_int(currLine->Default);
							valueIn = std::min(std::max(valueIn, (int)currLine->Min), (int)currLine->Max);
							currLine->Default = valueIn;
							currLine->Value = valueIn;
						}
					}
					break;
				}
				case FLOATING_LINE:
				{
					pugi::xml_node defaultValNode = lineFindItr->second.child(xmlTagConstants::valueDefaultTag.c_str());
					if (defaultValNode)
					{
						pugi::xml_attribute defaultValueAttr = defaultValNode.attribute(xmlTagConstants::valueTag.c_str());
						if (defaultValueAttr)
						{
							float valueIn = defaultValueAttr.as_float(GetFloatFromHex(currLine->Default));
							float maxVal = GetFloatFromHex(currLine->Max);
							float minVal = GetFloatFromHex(currLine->Min);
							valueIn = std::min(std::max(valueIn, minVal), maxVal);
							currLine->Default = GetHexFromFloat(valueIn);
							currLine->Value = currLine->Default;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
			// Lastly, check if the line is explicitly marked as locked...
			if (lineFindItr->second.attribute(xmlTagConstants::lockedTag.c_str()).as_bool(0))
			{
				// ... and if so, mark it as unselectable.
				currLine->setIsSelectable(0);
			}
		}

		// Lastly, check if the page is explicitly marked as locked...
		if (pageFindItr->second.attribute(xmlTagConstants::lockedTag.c_str()).as_bool(0))
		{
			// ... and if so, mark it as unselectable.
			currPage->CalledFromLine.setIsSelectable(0);
		}
	}
	// And lastly, for each page...
	for (Page* currPage : Pages)
	{
		// ... re-connect the lines in them, to ensure that anything made newly unselectable actually honors that designation.
		currPage->ConnectSelectableLines();
	}
}
bool applyLineSettingsFromMenuOptionsTree(Page& mainPageIn, std::string xmlPathIn)
{
	bool result = 0;

	pugi::xml_document tempDoc;
	if (loadMenuOptionsTree(xmlPathIn, tempDoc))
	{
		result = 1;
		applyLineSettingsFromMenuOptionsTree(mainPageIn, tempDoc);
	}

	return result;
}
bool buildMenuOptionsTreeFromMenu(Page& mainPageIn, std::string xmlPathOut)
{
	bool result = 1;

	pugi::xml_document MenuOptionsTree;

	pugi::xml_node commentNode = MenuOptionsTree.append_child(pugi::node_comment);
	commentNode.set_value("PowerPC Assembly Functions (Code Menu Building Utility)");
	commentNode = MenuOptionsTree.append_child(pugi::node_comment);
	commentNode.set_value("Important Note: Only change values noted as editable! Changing anything else will not work!");

	pugi::xml_node menuBaseNode = MenuOptionsTree.append_child(xmlTagConstants::codeMenuTag.c_str());
	pugi::xml_attribute menuPathAttr = menuBaseNode.append_attribute(xmlTagConstants::cmnuPathTag.c_str());
	menuPathAttr.set_value(getCMNUAbsolutePath().c_str());

	std::vector<Page*> Pages{ &mainPageIn };
	recursivelyFindPages(mainPageIn, Pages);

	for (unsigned long i = 0; i < Pages.size(); i++)
	{
		const Page* currPage = Pages[i];

		pugi::xml_node pageNode = menuBaseNode.append_child(xmlTagConstants::pageTag.c_str());
		pugi::xml_attribute pageNameAttr = pageNode.append_attribute(xmlTagConstants::nameTag.c_str());
		pageNameAttr.set_value(currPage->PageName.c_str());
		if (!currPage->CalledFromLine.getIsSelectable())
		{
			pageNode.append_attribute(xmlTagConstants::lockedTag.c_str()).set_value("true");
		}

		for (unsigned long u = 0; u < currPage->Lines.size(); u++)
		{
			const Line* currLine = currPage->Lines[u];

			std::vector<const char*> deconstructedText = splitLineContentString(currLine->Text);
			switch (currLine->type)
			{
				case SELECTION_LINE:
				{
					pugi::xml_node lineNode = pageNode.append_child(xmlTagConstants::selectionTag.c_str());
					pugi::xml_attribute lineNameAttr = lineNode.append_attribute(xmlTagConstants::nameTag.c_str());
					lineNameAttr.set_value(deconstructedText[0]);
					if (!currLine->getIsSelectable())
					{
						lineNode.append_attribute(xmlTagConstants::lockedTag.c_str()).set_value("true");
					}
					pugi::xml_node defaultValNode = lineNode.append_child(xmlTagConstants::selectionDefaultTag.c_str());
					defaultValNode.append_attribute(xmlTagConstants::indexTag.c_str()).set_value(std::to_string(currLine->Default).c_str());
					defaultValNode.append_attribute(xmlTagConstants::editableTag.c_str()).set_value("true");
					for (unsigned long i = 1; i < deconstructedText.size(); i++)
					{
						pugi::xml_node optionNode = lineNode.append_child(xmlTagConstants::selectionOptionTag.c_str());
						pugi::xml_attribute optionValueAttr = optionNode.append_attribute(xmlTagConstants::valueTag.c_str());
						optionValueAttr.set_value(deconstructedText[i]);
					}
					break;
				}
				case INTEGER_LINE:
				{
					pugi::xml_node lineNode = pageNode.append_child(xmlTagConstants::intTag.c_str());
					pugi::xml_attribute lineNameAttr = lineNode.append_attribute(xmlTagConstants::nameTag.c_str());
					lineNameAttr.set_value(deconstructedText[0]);
					if (!currLine->getIsSelectable())
					{
						lineNode.append_attribute(xmlTagConstants::lockedTag.c_str()).set_value("true");
					}
					pugi::xml_node minValNode = lineNode.append_child(xmlTagConstants::valueMinTag.c_str());
					minValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(currLine->Min).c_str());
					pugi::xml_node defaultValNode = lineNode.append_child(xmlTagConstants::valueDefaultTag.c_str());
					defaultValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(currLine->Default).c_str());
					defaultValNode.append_attribute(xmlTagConstants::editableTag.c_str()).set_value("true");
					pugi::xml_node maxValNode = lineNode.append_child(xmlTagConstants::valueMaxTag.c_str());
					maxValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(currLine->Max).c_str());
					break;
				}
				case FLOATING_LINE:
				{
					pugi::xml_node lineNode = pageNode.append_child(xmlTagConstants::floatTag.c_str());
					pugi::xml_attribute lineNameAttr = lineNode.append_attribute(xmlTagConstants::nameTag.c_str());
					lineNameAttr.set_value(deconstructedText[0]);
					if (!currLine->getIsSelectable())
					{
						lineNode.append_attribute(xmlTagConstants::lockedTag.c_str()).set_value("true");
					}
					pugi::xml_node minValNode = lineNode.append_child(xmlTagConstants::valueMinTag.c_str());
					minValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(GetFloatFromHex(currLine->Min)).c_str());
					pugi::xml_node defaultValNode = lineNode.append_child(xmlTagConstants::valueDefaultTag.c_str());
					defaultValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(GetFloatFromHex(currLine->Default)).c_str());
					defaultValNode.append_attribute(xmlTagConstants::editableTag.c_str()).set_value("true");
					pugi::xml_node maxValNode = lineNode.append_child(xmlTagConstants::valueMaxTag.c_str());
					maxValNode.append_attribute(xmlTagConstants::valueTag.c_str()).set_value(std::to_string(GetFloatFromHex(currLine->Max)).c_str());
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	MenuOptionsTree.save_file(xmlPathOut.c_str());

	return result;
}



void CodeMenu()
{
#if EON_DEBUG_BUILD
	//testing page
	vector<Line*> TestLines;
	//P2Lines.push_back(new Selection("P2 Identity Crisis", CHARACTER_LIST, CHARACTER_ID_LIST, 0, CHARACTER_SELECT_P2_INDEX));
	cout << "Debug Toggles:\n";
	int toggleLocations[16];
	for(int i = 0; i < 16; i++)
	{
		TestLines.push_back(new Integer(to_string(i), -10000, 10000, 0, 1, toggleLocations[i]));
	}
	Page TestPage("Testing flags", TestLines);
#endif
	
	//player pages
	vector<Line*> P1Lines;
	P1Lines.push_back(new Toggle("Infinite Shield", false, INFINITE_SHIELDS_P1_INDEX));
	Selection* P1CharSelect = new Selection("P1 Character Select", CHARACTER_LIST, CHARACTER_ID_LIST, 0, CHARACTER_SELECT_P1_INDEX);
	P1Lines.push_back(P1CharSelect);
	//P1Lines.push_back(new Selection("P1 Identity Crisis", CHARACTER_LIST, CHARACTER_ID_LIST, 0, CHARACTER_SELECT_P1_INDEX));
	P1Lines.push_back(new Floating("Select Percent", 0, 999, 0, 1, PERCENT_SELECT_VALUE_P1_INDEX, "%.0f%%"));
	P1Lines.push_back(new Toggle("Press DPad to select percent", false, PERCENT_SELECT_ACTIVATOR_P1_INDEX));
	P1Lines.push_back(new Toggle("Disable DPad", false, DISABLE_DPAD_P1_INDEX));
	if (PROJECT_PLUS_EX_BUILD)
	{
		P1Lines.push_back(new Selection("Input Buffer", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }, 0, BUFFER_P1_INDEX));
		P1Lines.push_back(new Selection("Automatic L-Cancelling", { "OFF", "ON", "Modified" }, 0, ALC_P1_INDEX));
		P1Lines.push_back(new Floating("ALC Modifier", 0.099, 3, 0.5, 0.05, EXTERNAL_INDEX, "%.2fX"));
		P1Lines.push_back(new Toggle("Red Flash on L-Cancel Failure", false, ALC_P1_FLASH_RED_INDEX));
	}
	P1Lines.push_back(new Comment(""));
	P1Lines.push_back(new Print("Tag Hex: %s", { &P1_TAG_STRING_INDEX }));
	P1Lines.push_back(new Comment("For Use With Tag-Based Costumes"));

	for (auto x : P1Lines) {
		cout << x->Text << endl;
	}
	Page P1("Player 1 Codes", P1Lines);

	vector<Line*> P2Lines;
	P2Lines.push_back(new Toggle("Infinite Shield", false, INFINITE_SHIELDS_P2_INDEX));
	P2Lines.push_back(new SelectionMirror(*P1CharSelect, "P2 Character Select", 0, CHARACTER_SELECT_P2_INDEX));
	P2Lines.push_back(new Floating("Select Percent", 0, 999, 0, 1, PERCENT_SELECT_VALUE_P2_INDEX, "%.0f%%"));
	P2Lines.push_back(new Toggle("Press DPad to select percent", false, PERCENT_SELECT_ACTIVATOR_P2_INDEX));
	P2Lines.push_back(new Toggle("Disable DPad", false, DISABLE_DPAD_P2_INDEX));
	if (PROJECT_PLUS_EX_BUILD)
	{
		P2Lines.push_back(new Selection("Input Buffer", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }, 0, BUFFER_P2_INDEX));
		P2Lines.push_back(new Selection("Automatic L-Cancelling", { "OFF", "ON", "Modified" }, 0, ALC_P2_INDEX));
		P2Lines.push_back(new Floating("ALC Modifier", 0.099, 3, 0.5, 0.05, EXTERNAL_INDEX, "%.2fX"));
		P2Lines.push_back(new Toggle("Red Flash on L-Cancel Failure", false, ALC_P2_FLASH_RED_INDEX));
	}
	P2Lines.push_back(new Comment(""));
	P2Lines.push_back(new Print("Tag Hex: %s", { &P2_TAG_STRING_INDEX }));
	P2Lines.push_back(new Comment("For Use With Tag-Based Costumes"));
	Page P2("Player 2 Codes", P2Lines);

	vector<Line*> P3Lines;
	P3Lines.push_back(new Toggle("Infinite Shield", false, INFINITE_SHIELDS_P3_INDEX));
	P3Lines.push_back(new SelectionMirror(*P1CharSelect, "P3 Character Select", 0, CHARACTER_SELECT_P3_INDEX));
	P3Lines.push_back(new Floating("Select Percent", 0, 999, 0, 1, PERCENT_SELECT_VALUE_P3_INDEX, "%.0f%%"));
	P3Lines.push_back(new Toggle("Press DPad to select percent", false, PERCENT_SELECT_ACTIVATOR_P3_INDEX));
	P3Lines.push_back(new Toggle("Disable DPad", false, DISABLE_DPAD_P3_INDEX));
	if (PROJECT_PLUS_EX_BUILD)
	{
		P3Lines.push_back(new Selection("Input Buffer", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }, 0, BUFFER_P3_INDEX));
		P3Lines.push_back(new Selection("Automatic L-Cancelling", { "OFF", "ON", "Modified" }, 0, ALC_P3_INDEX));
		P3Lines.push_back(new Floating("ALC Modifier", 0.099, 3, 0.5, 0.05, EXTERNAL_INDEX, "%.2fX"));
		P3Lines.push_back(new Toggle("Red Flash on L-Cancel Failure", false, ALC_P3_FLASH_RED_INDEX));
	}
	P3Lines.push_back(new Comment(""));
	P3Lines.push_back(new Print("Tag Hex: %s", { &P3_TAG_STRING_INDEX }));
	P3Lines.push_back(new Comment("For Use With Tag-Based Costumes"));
	Page P3("Player 3 Codes", P3Lines);

	vector<Line*> P4Lines;
	P4Lines.push_back(new Toggle("Infinite Shield", false, INFINITE_SHIELDS_P4_INDEX));
	P4Lines.push_back(new SelectionMirror(*P1CharSelect, "P4 Character Select", 0, CHARACTER_SELECT_P4_INDEX));
	P4Lines.push_back(new Floating("Select Percent", 0, 999, 0, 1, PERCENT_SELECT_VALUE_P4_INDEX, "%.0f%%"));
	P4Lines.push_back(new Toggle("Press DPad to select percent", false, PERCENT_SELECT_ACTIVATOR_P4_INDEX));
	P4Lines.push_back(new Toggle("Disable DPad", false, DISABLE_DPAD_P4_INDEX));
	if (PROJECT_PLUS_EX_BUILD)
	{
		P4Lines.push_back(new Selection("Input Buffer", { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" }, 0, BUFFER_P4_INDEX));
		P4Lines.push_back(new Selection("Automatic L-Cancelling", { "OFF", "ON", "Modified" }, 0, ALC_P4_INDEX));
		P4Lines.push_back(new Floating("ALC Modifier", 0.099, 3, 0.5, 0.05, EXTERNAL_INDEX, "%.2fX"));
		P4Lines.push_back(new Toggle("Red Flash on L-Cancel Failure", false, ALC_P4_FLASH_RED_INDEX));
	}
	P4Lines.push_back(new Comment(""));
	P4Lines.push_back(new Print("Tag Hex: %s", { &P4_TAG_STRING_INDEX }));
	P4Lines.push_back(new Comment("For Use With Tag-Based Costumes"));
	Page P4("Player 4 Codes", P4Lines);

	//debug mode
	vector<Line*> DebugLines;
	DebugLines.push_back(new Comment("Debug Mode Commands:"));
	DebugLines.push_back(new Comment("Start = Toggle Frame Advance"));
	DebugLines.push_back(new Comment("Z = Frame Advance | Hold Z = Slow Motion"));
	DebugLines.push_back(new Comment(""));
	DebugLines.push_back(new Toggle("Debug Mode", false, DEBUG_MODE_INDEX));
	DebugLines.push_back(new Selection("Hitbox Display", { "OFF", "ON", "Models Off" }, 0, DISPLAY_HITBOXES_INDEX));
	DebugLines.push_back(new Toggle("Collision Display", false, DISPLAY_COLLISION_INDEX));
	DebugLines.push_back(new Selection("Stage Collisions", { "OFF", "ON", "Background Off" }, 0, DISPLAY_LEDGEGRAB_INDEX));
	DebugLines.push_back(new Toggle("Camera Lock", false, CAMERA_LOCK_INDEX));
	DebugLines.push_back(new Toggle("Draw DI", false, DI_DRAW_INDEX));
	DebugLines.push_back(new Toggle("FPS Display", false, FPS_DISPLAY_INDEX));
	DebugLines.push_back(new Toggle("HUD", true, HUD_DISPLAY_INDEX));
	Page DebugMode("Debug Mode Settings", DebugLines);

	//value setting
	vector<Line*> ConstantsLines;
	ConstantsLines.push_back(new Comment("Set attributes to new values"));
	ConstantsLines.push_back(new Comment(""));
	ConstantsLines.push_back(new Floating("Hitstun Multiplier", 0, 999, 0.4, .01, HITSTUN_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B87AA8, HITSTUN_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Hitlag Multiplier", 0, 999, 1. / 3., .02, HITLAG_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B87AEC, HITLAG_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Hitlag Maximum", 0, 999, 30, 1, HITLAG_MAXIMUM_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B87AE8, HITLAG_MAXIMUM_INDEX);
	ConstantsLines.push_back(new Floating("Electric Hitlag Multiplier", 0, 999, 1.5, .1, ELECTRIC_HITLAG_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B87B10, ELECTRIC_HITLAG_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("SDI Distance", -999, 999, 6, .5, SDI_DISTANCE_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88354, SDI_DISTANCE_INDEX);
	ConstantsLines.push_back(new Floating("ASDI Distance", -999, 999, 3, .5, ASDI_DISTANCE_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88358, ASDI_DISTANCE_INDEX);
	ConstantsLines.push_back(new Floating("Walljump Horizontal Multiplier", -999, 999, 0.9, .05, WALLJUMP_HORIZONTAL_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88420, WALLJUMP_HORIZONTAL_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Minimum Shield Size Scale", -999, 999, 0.15, .02, MINIMUM_SHIELD_SIZE_SCALING_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88444, MINIMUM_SHIELD_SIZE_SCALING_INDEX);
	ConstantsLines.push_back(new Floating("Shield Damage Multiplier", -999, 999, 1, .02, SHIELD_DAMAGE_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B8845C, SHIELD_DAMAGE_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Base Shield Damage", -999, 999, 0, 1, SHIELD_BASE_DAMAGE_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88460, SHIELD_BASE_DAMAGE_INDEX);
	ConstantsLines.push_back(new Floating("Shield Size Multiplier", -999, 999, 1, .05, SHIELD_SIZE_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88478, SHIELD_SIZE_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Shield Tilt Multiplier", -999, 999, 0.5, .05, SHIELD_TILT_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88484, SHIELD_TILT_MULTIPLIER_INDEX);
	//ValueLines.push_back(new Floating("Attacker Shield Pushback Friction Multiplier", -999, 999, 1.1, .05, SDI_DISTANCE_INDEX, "%.3f"));
	ConstantsLines.push_back(new Floating("Wall Bounce Knockback Multiplier", -999, 999, 0.80, .02, WALL_BOUNCE_KNOCKBACK_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88510, WALL_BOUNCE_KNOCKBACK_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Floating("Knockback Decay Rate", -999, 999, 0.051, .001, KNOCKBACK_DECAY_MULTIPLIER_INDEX, "%.3f"));
	constantOverrides.emplace_back(0x80B88534, KNOCKBACK_DECAY_MULTIPLIER_INDEX);
	ConstantsLines.push_back(new Selection("Staling Toggle", { "Default", "ON", "OFF" }, 0, STALING_TOGGLE_INDEX));
	if (CONFIG_DASH_ATTACK_ITEM_GRAB_ENABLED)
	{
		ConstantsLines.push_back(new Selection("Aerial & Dash Attack Item Grab Toggle", { "OFF", "ON" }, 0, DASH_ATTACK_ITEM_GRAB_INDEX));
	}
	//ConstantsLines.push_back(new Selection("Tripping Toggle", { "OFF", "ON" }, 0, TRIP_TOGGLE_INDEX));
	//ConstantsLines.push_back(new Floating("Tripping Rate", 0, 100, 1.0, 1.0, TRIP_RATE_MULTIPLIER_INDEX, "%.2f%"));
	//ConstantsLines.push_back(new Selection("Tripping Cooldown Toggle", { "ON", "OFF" }, 0, TRIP_INTERVAL_INDEX));
	if (CONFIG_JUMPSQUAT_OVERRIDE_ENABLED)
	{
		const int minFrameCount = 1;
		const int maxFrameCount = 3600;
		ConstantsLines.push_back(new Selection("Jumpsquat Modifier Mode", { "Disabled", "Replace", "Add", "Subtract", "Multiply", "Divide", "Distance From Value", "Random Within Range"}, 0, JUMPSQUAT_OVERRIDE_TOGGLE_INDEX));
		ConstantsLines.push_back(new Integer("Jumpsquat Modifier Value", minFrameCount, maxFrameCount, 3, 1, JUMPSQUAT_OVERRIDE_FRAMES_INDEX, "%d", Integer::INT_FLAG_ALLOW_WRAP));
		ConstantsLines.push_back(new Integer("Jumpsquat Modifier Minimum Length", minFrameCount, maxFrameCount, minFrameCount, 1, JUMPSQUAT_OVERRIDE_MIN_INDEX, "%d", Integer::INT_FLAG_ALLOW_WRAP));
		ConstantsLines.push_back(new Integer("Jumpsquat Modifier Maximum Length", minFrameCount, maxFrameCount, maxFrameCount, 1, JUMPSQUAT_OVERRIDE_MAX_INDEX, "%d", Integer::INT_FLAG_ALLOW_WRAP));
	}
	Page ConstantsPage("Gameplay Modifiers", ConstantsLines);

	//DBZ Mode settings
	vector<Line*> DBZModeLines;
	DBZModeLines.push_back(new Toggle("Flight Mode", false, DBZ_MODE_INDEX));
	DBZModeLines.push_back(new Floating("Max Horizontal Speed", 0, 100, 2, .05, DBZ_MODE_MAX_SPEED_X_INDEX, "%.3f"));
	DBZModeLines.push_back(new Floating("Max Vertical Speed", 0, 100, 2, .05, DBZ_MODE_MAX_SPEED_Y_INDEX, "%.3f"));
	DBZModeLines.push_back(new Comment("Acceleration Scales Based On Stick Magnitude"));
	DBZModeLines.push_back(new Floating("Horizontal Acceleration", -100, 100, 1, .01, DBZ_MODE_ACCEL_X_INDEX, "%.3f"));
	DBZModeLines.push_back(new Floating("Vertical Acceleration", -100, 100, 1, .01, DBZ_MODE_ACCEL_Y_INDEX, "%.3f"));
	Page DBZModePage("Flight Mode Settings", DBZModeLines);
	
	//Special Mode Settings
	vector<Line*> SpecialModeLines;
	SpecialModeLines.push_back(new Comment("Special Modes"));
	SpecialModeLines.push_back(&ConstantsPage.CalledFromLine);
	SpecialModeLines.push_back(&DBZModePage.CalledFromLine);
	SpecialModeLines.push_back(new Toggle("Random Angle Mode", false, RANDOM_ANGLE_INDEX));
	if (PROJECT_PLUS_EX_BUILD)
	{
		SpecialModeLines.push_back(new Toggle("War Mode", false, WAR_MODE_INDEX));
		SpecialModeLines.push_back(new Selection("Gameplay Speed Modifier", { "Off", "1.25", "1.5x", "2.0x", "1/2x", "3/4x" }, 0, SPEED_INDEX));
		SpecialModeLines.push_back(new Toggle("Scale Mode", false, SCALE_INDEX));
		SpecialModeLines.push_back(new Floating("Scale Modifier", 0.5, 3, 1, 0.05, EXTERNAL_INDEX, "%.2fX"));
	}
	SpecialModeLines.push_back(new Selection("Big Head Mode", { "Off", "On", "Larger", "Largest", "Largerest" }, 0, BIG_HEAD_INDEX));
	Page SpecialModePage("Special Modes", SpecialModeLines);


	

	//main page
	vector<Line*> MainLines;
	// Writes the Menu's Title, appending the netplay suffix only if the config file didn't specify its own title.
	MainLines.push_back(new Comment(MENU_NAME + ((!CUSTOM_NAME_SUPPLIED && BUILD_NETPLAY_FILES) ? " (Netplay)" : "")));
	if (!CONFIG_DELETE_CONTROLS_COMMENTS)
	{
		MainLines.push_back(new Comment("Green = Comments | Blue = Changed"));
		MainLines.push_back(new Comment("A = Enter Submenu | B = Back/Exit"));
		MainLines.push_back(new Comment("X = Reset Selection | Y = Reset Page"));
		MainLines.push_back(new Comment("Hold Z = Scroll Faster"));
		MainLines.push_back(new Comment(""));
	}
	for (std::size_t i = 0; i < CONFIG_INCOMING_COMMENTS.size(); i++)
	{
		MainLines.push_back(new Comment(CONFIG_INCOMING_COMMENTS[i]));
	}
	

#if EON_DEBUG_BUILD
	MainLines.push_back(&TestPage.CalledFromLine);
#endif
	
	MainLines.push_back(&DebugMode.CalledFromLine);
	//	MainLines.push_back(new Selection("Endless Friendlies", { "OFF", "Same Stage", "Random Stage", "Round Robin" }, 0, INFINITE_FRIENDLIES_INDEX));
	MainLines.push_back(new Selection("Endless Friendlies Mode", { "OFF", "All Stay", "Winner Stays", "Loser Stays", "Rotation"}, 0, ENDLESS_FRIENDLIES_MODE_INDEX));
	MainLines.push_back(new Selection("Endless Friendlies Stage Selection", { "Random", "Same" }, 0, ENDLESS_FRIENDLIES_STAGE_SELECTION_INDEX));
#if TOURNAMENT_ADDITION_BUILD
	MainLines.push_back(new Selection("Random 1-1", { "OFF", "ON" }, 0, RANDOM_1_TO_1_INDEX));
#endif
	MainLines.push_back(new Selection("Alternate Stages", { "Enabled", "Random", "OFF" }, 0, ALT_STAGE_BEHAVIOR_INDEX));
	MainLines.push_back(new Toggle("Autoskip Results Screen", false, AUTO_SKIP_TO_CSS_INDEX));
#if DOLPHIN_BUILD
	MainLines.push_back(new Toggle("Autosave Replays", true, AUTO_SAVE_REPLAY_INDEX));
#else
	MainLines.push_back(new Toggle("Autosave Replays", false, AUTO_SAVE_REPLAY_INDEX));
#endif
	MainLines.push_back(new Toggle("Save Previous Replay", false, SAVE_REPLAY_ANYWHERE_INDEX));
	MainLines.push_back(new Selection("Tag-Based Costumes", { "ON", "ON + Teams", "OFF" }, 0, TAG_COSTUME_TOGGLE_INDEX));
	MainLines.push_back(&P1.CalledFromLine);
	MainLines.push_back(&P2.CalledFromLine);
	MainLines.push_back(&P3.CalledFromLine);
	MainLines.push_back(&P4.CalledFromLine);
	MainLines.push_back(&SpecialModePage.CalledFromLine);

	
	// HUD Color Settings
	vector<Line*> HUDColorLines;
	HUDColorLines.push_back(new Comment("Replacement Hud Colors:"));
	HUDColorLines.push_back(new Integer("Red",		0, BACKPLATE_COLOR_TOTAL_COLOR_COUNT - 1, 1, 1, BACKPLATE_COLOR_1_INDEX, "Color %d", Integer::INT_FLAG_ALLOW_WRAP));
	HUDColorLines.push_back(new Integer("Blue",		0, BACKPLATE_COLOR_TOTAL_COLOR_COUNT - 1, 2, 1, BACKPLATE_COLOR_2_INDEX, "Color %d", Integer::INT_FLAG_ALLOW_WRAP));
	HUDColorLines.push_back(new Integer("Yellow",	0, BACKPLATE_COLOR_TOTAL_COLOR_COUNT - 1, 3, 1, BACKPLATE_COLOR_3_INDEX, "Color %d", Integer::INT_FLAG_ALLOW_WRAP));
	HUDColorLines.push_back(new Integer("Green",	0, BACKPLATE_COLOR_TOTAL_COLOR_COUNT - 1, 4, 1, BACKPLATE_COLOR_4_INDEX, "Color %d", Integer::INT_FLAG_ALLOW_WRAP));
	HUDColorLines.push_back(new Integer("Gray",		9, 9, 9, 0, BACKPLATE_COLOR_C_INDEX, "Color %d")); // Note: Cannot be changed, on purpose.
	HUDColorLines.back()->setIsSelectable(0);
	HUDColorLines.push_back(new Integer("Clear",	0, 0, 0, 0, BACKPLATE_COLOR_T_INDEX, "Color %d")); // Note: Cannot be changed, on purpose.
	HUDColorLines.back()->setIsSelectable(0);
	Page HUDColorsPage("HUD Colors", HUDColorLines);
	if ((CONFIG_BACKPLATE_COLOR_MODE > 0) && (CONFIG_BACKPLATE_COLOR_MODE < backplateColorConstants::pSCL__COUNT))
	{
		MainLines.push_back(&HUDColorsPage.CalledFromLine);
	}


#if BUILD_TYPE == PROJECT_PLUS
	MainLines.push_back(new Toggle("Crowd Cheers", false, CROWD_CHEER_TOGGLE_INDEX));
#endif

	MainLines.push_back(new Selection("Code Menu Activation", { "Default", "PM 3.6", "OFF" }, 0, CODE_MENU_ACTIVATION_SETTING_INDEX));
	if (ROSTER_LIST.size() > 1)
	{
		MainLines.push_back(new Selection("CSS Roster Version", ROSTER_LIST, 0, CSS_VERSION_SETTING_INDEX));
	}

	if (THEME_LIST.size() > 1)
	{
		MainLines.push_back(new Selection("Active Theme", THEME_LIST, 0, THEME_SETTING_INDEX));
	}
	
	//MainLines.push_back(new Print("%s", {&tets}));
	
	
	/*MainLines.push_back(new Integer("P1 1st Shield Red", 0, 0xFF, 0, 1, SHIELD_RED_1));
	MainLines.push_back(new Integer("P1 1st Shield Green", 0, 0xFF, 0, 1, SHIELD_GREEN_1));
	MainLines.push_back(new Integer("P1 1st Shield Blue", 0, 0xFF, 0, 1, SHIELD_BLUE_1));
	MainLines.push_back(new Integer("P1 1st Shield Alpha", 0, 0xFF, 0, 1, SHIELD_ALPHA_1));
	MainLines.push_back(new Integer("P1 2nd Shield Red", 0, 0xFF, 0, 1, SHIELD_RED_2));
	MainLines.push_back(new Integer("P1 2nd Shield Green", 0, 0xFF, 0, 1, SHIELD_GREEN_2));
	MainLines.push_back(new Integer("P1 2nd Shield Blue", 0, 0xFF, 0, 1, SHIELD_BLUE_2));
	MainLines.push_back(new Integer("P1 2nd Shield Alpha", 0, 0xFF, 0, 1, SHIELD_ALPHA_2));
	MainLines.push_back(new Integer("P1 3rd Shield Red", 0, 0xFF, 0, 1, SHIELD_RED_3));
	MainLines.push_back(new Integer("P1 3rd Shield Green", 0, 0xFF, 0, 1, SHIELD_GREEN_3));
	MainLines.push_back(new Integer("P1 3rd Shield Blue", 0, 0xFF, 0, 1, SHIELD_BLUE_3));
	MainLines.push_back(new Integer("P1 3rd Shield Alpha", 0, 0xFF, 0, 1, SHIELD_ALPHA_3));
	MainLines.push_back(new Integer("P1 4th Shield Red", 0, 0xFF, 0, 1, SHIELD_RED_4));
	MainLines.push_back(new Integer("P1 4th Shield Green", 0, 0xFF, 0, 1, SHIELD_GREEN_4));
	MainLines.push_back(new Integer("P1 4th Shield Blue", 0, 0xFF, 0, 1, SHIELD_BLUE_4));
	MainLines.push_back(new Integer("P1 4th Shield Alpha", 0, 0xFF, 0, 1, SHIELD_ALPHA_4));*/



	Page Main("Main", MainLines);
	
	//Unclepunch fps code
	CodeRaw("[CM: Code Menu] FPS Code [UnclePunch]", "", {
		3254926684, 3, 2288895028, 946012161, 2557330484, 2283733000, 1610612736, 0, 3254926716, 6, 2288895029, 946012161, 2557330485, 738394172, 1098907672, 2288895028, 2959983670, 945815552, 2557330484, 2557330485, 2147549204, 0
		});

	/*
	Causes target smash to load lvl 1.  Fixed by Duke in seperate code
	WriteIntToFile(0xC210F98C);
	WriteIntToFile(0x00000002);
	WriteIntToFile(0x7C9F2378);
	WriteIntToFile(0x3BA00000);
	WriteIntToFile(0x93BF0000);
	WriteIntToFile(0x00000000);*/

	CreateMenu(Main);

	PrintCodeMenu();

	PrimeCodeMenu();

	ControlCodeMenu();

	ActualCodes();

#if EON_DEBUG_BUILD
	for(auto TOGGLE_LOC: toggleLocations)
	{
		printf("%0X\n", TOGGLE_LOC);
	}
	cout << endl;
#endif
	
	//printf("%X", P1_TAG_STRING_INDEX);

	//printf("%0Xu\n", KnucklesTemp + 8);

	MenuFile.close();
}

void printMenuSetters() {

	//Could probably set the string indexes to the hex buffers during compile, since they never change

	int resultPtrReg = 31;
	int tagPtrReg = 30;
	int hexCharReg = 29;
	int stringReg = 28;
	int tagMenuReg = 27;
	int tagMenuPtrReg = 26;
	int resultHexStartReg = 25;
	int counterReg = 24;

	//set string to convert
	SetRegister(3, "%02X");
	SetRegister(stringReg, STRING_BUFFER + 0x80);
	STW(3, stringReg, 0);
	SetRegister(3, 0);
	STW(3, stringReg, 4);

	SetRegister(resultHexStartReg, P1_TAG_HEX_LOC + 1);

	SetRegister(tagMenuPtrReg, 0x805882e0 - 4); //this needs wiiztec's 0 to death code

	for (auto tagIndex : { P1_TAG_STRING_INDEX, P2_TAG_STRING_INDEX, P3_TAG_STRING_INDEX, P4_TAG_STRING_INDEX }) {
		if (tagIndex != -1) {
			MR(resultPtrReg, resultHexStartReg);

			//get menu location
			LWZU(tagMenuReg, tagMenuPtrReg, 4);
			If(tagMenuReg, NOT_EQUAL_I, 0); {
				ADDI(tagMenuReg, tagMenuReg, 0x1FC);
				LWZ(3, tagMenuReg, MENU_POS_OFFSET);
				ADDI(3, 3, -1);
				//check if initialized
				If(3, LESS_I_L, 120); {
					//get tag location
					MULLI(3, 3, 2);
					ADDI(3, 3, MENU_INDEX_OFFSET);
					LHZX(3, tagMenuReg, 3);
					If(3, LESS_I_L, 120); {
						MULLI(3, 3, TAG_LIST_SIZE);
						SetRegister(tagPtrReg, TAG_LIST_START_LOC);
						ADD(tagPtrReg, tagPtrReg, 3);

						//convert tag hex to string
						//check if entire half is not 0, then convert byte.  Might be able to just convert half, doesn't really matter
						SetRegister(counterReg, 0);
						LHZ(hexCharReg, tagPtrReg, 0);
						While(hexCharReg, NOT_EQUAL_I, 0); {
							//don't loop too many times
							If(counterReg, LESS_OR_EQUAL_I, 4); {
								LBZ(hexCharReg, tagPtrReg, 0);
								MR(3, resultPtrReg);
								MR(4, stringReg);
								MR(5, hexCharReg);
								WriteIntToFile(0x4cc63182); //clclr 6, 6
								CallBrawlFunc(0x803f89fc); //sprintf

								ADD(resultPtrReg, resultPtrReg, 3);
								LHZU(hexCharReg, tagPtrReg, 1);
							} Else(); {
								//exit
								SetRegister(hexCharReg, 0);
							} EndIf();
						} EndWhile();
					} EndIf();
				} EndIf();
			} EndIf();

			//null terminator
			SetRegister(3, 0);
			STB(3, resultPtrReg, 0);

			SetRegister(3, tagIndex);
			ADDI(4, resultHexStartReg, -1); //move back one for $
			STW(4, 3, 0);

			//next sting buffer
			ADDI(resultHexStartReg, resultHexStartReg, P2_TAG_HEX_LOC - P1_TAG_HEX_LOC);
		}
	}
}

void stopAnouncer() {
	ASMStart(0x809580b4, "[CM: Code Menu] Stop Announcer");
	//SaveRegisters();
	
	int reg1 = 4;

	LoadWordToReg(reg1, ENDLESS_FRIENDLIES_MODE_INDEX + Line::VALUE);
	If(reg1, GREATER_OR_EQUAL_I, 2); {
		BLR();
	} EndIf();

	//RestoreRegisters();
	ASMEnd(0x9421ffe0); //stwu sp, -0x20 (sp)
}

void endlessFriendlies() {
	stopAnouncer();

	//r3 + 0x5D is flag
	//r3 + 0x28 ^ 0x20 is port num (0 based)
	ASMStart(0x809489ec, "[CM: Code Menu] Endless Friendlies");
	SaveRegisters();

	int reg1 = 31;
	int reg2 = 30;
	int reg3 = 29;
	int reg4 = 28;
	int reg5 = 27;
	int reg6 = 26;
	int flagReg = 25;

	
	SetRegister(flagReg, -1); //Doesn't do anything unless changed

	LoadWordToReg(reg1, ENDLESS_FRIENDLIES_MODE_INDEX + Line::VALUE);
	If(reg1, GREATER_OR_EQUAL_I, 2); {
		LoadWordToReg(reg2, reg1, ENDLESS_ROTATION_QUEUE_LOC);
		If(reg2, NOT_EQUAL_I, 0); {
			SetRegister(flagReg, 1);

			LWZ(reg3, 3, 0x48);
			ADDI(reg3, reg3, 1); //port num

			SetRegister(reg4, P1_STOP_LOAD_FLAG_PTR_LOC - 4);
			MULLI(reg5, reg3, 4);
			ADDI(reg6, 3, 0x5D);
			STWX(reg6, reg4, reg5);

			RLWINM(reg4, reg2, 8, 24, 31); //>>24
			If(reg4, EQUAL, reg3); {
				SetRegister(flagReg, 0);
			} EndIf();

			RLWINM(reg4, reg2, 16, 24, 31); //>>16 & 0xFF
			If(reg4, EQUAL, reg3); {
				SetRegister(flagReg, 0);
			} EndIf();
		} Else(); {
			IfInVersus(reg3); {
				SetRegister(flagReg, 0);
			} EndIf();
		} EndIf();
	} EndIf();

	If(flagReg, EQUAL_I, 0);
	{
		LBZ(reg3, 3, 0x5D);
		ANDI(reg3, reg3, ~0x8); //clear flag
		STB(reg3, 3, 0x5D);
	} EndIf();
	If(flagReg, EQUAL_I, 1);
	{
		LBZ(reg3, 3, 0x5D);
		ORI(reg3, reg3, 0x8); //set flag
		STB(reg3, 3, 0x5D);
	} EndIf();
	

	RestoreRegisters();
	ASMEnd(0x8803005d); //lbz r0, 0x5D (r3)
}

void ActualCodes()
{
	endlessFriendlies();

	if (DI_DRAW_INDEX != -1) {
		DrawDI();
	}

	if (SAVE_STATES_INDEX != -1) {
		AddArticle();

		RemoveArticle();
	}

	if(!constantOverrides.empty()) {
		constantOverride();
	}

	if(DBZ_MODE_INDEX != -1) {
		DBZMode();
	}

	if(ALT_STAGE_BEHAVIOR_INDEX != -1) {
		//ASMStart(0x8094a168);
#if BUILD_TYPE == PROJECT_PLUS
		ASMStart(0x8010f990, "[CM: Code Menu] Alt Stage Behavior");
#else
		ASMStart(0x8094bf60);
#endif
		SaveRegisters();

		int Reg1 = 3;
		int Reg2 = 4;
		int Reg3 = 5;
		int Reg4 = 6;

		LoadWordToReg(Reg1, ALT_STAGE_BEHAVIOR_INDEX + Line::VALUE);
		If(Reg1, EQUAL_I, 2); //disable
		{
			SetRegister(Reg1, 0);
			SetRegister(Reg2, 0x800B9EA2);
			STH(Reg1, Reg2, 0);
			SetRegister(Reg2, ALT_STAGE_VAL_LOC);
			STH(Reg1, Reg2, 0);
		} Else(); If(Reg1, EQUAL_I, 1); //random
		{
#if BUILD_TYPE == PROJECT_PLUS
			vector<int> alts = { 0, BUTTON_L, BUTTON_R, BUTTON_Z, BUTTON_Y };
#else
			vector<int> alts = { 0, BUTTON_L, BUTTON_Z, BUTTON_START };
#endif
			LoadWordToReg(Reg1, Reg2, RANDOM_ALTS_MATCH_START_FLAG);
			If(Reg1, EQUAL_I, 1); {
				//set new rng value and clear flag
				LoadWordToReg(Reg4, 0x805a00bc); //random val
				//RLWINM(Reg1, Reg1, 32 - 8, 30, 31);
				SetRegister(Reg3, alts.size());
				MOD(Reg1, Reg4, Reg3);
				SetRegister(Reg3, RANDOM_ALTS_RNG);
				STW(Reg1, Reg3, 0);
				SetRegister(Reg1, 0);
				STW(Reg1, Reg2, 0);
			} EndIf();
			
			LoadWordToReg(Reg3, RANDOM_ALTS_RNG);

			//SetRegister(Reg2, 4);
			//MOD(Reg3, Reg1, Reg2);
			for (int i = 0; i < alts.size(); i++) {
				If(Reg3, EQUAL_I, i); {
					SetRegister(Reg4, alts[i]);
				} EndIf();
			}
			/*If(Reg3, EQUAL_I, 1);
			{
				SetRegister(Reg3, BUTTON_L);
			} EndIf();
			If(Reg3, EQUAL_I, 2);
			{
				SetRegister(Reg3, BUTTON_START);
			} EndIf();
			If(Reg3, EQUAL_I, 3);
			{
				SetRegister(Reg3, BUTTON_Z);
			} EndIf();*/
			SetRegister(Reg2, 0x800B9EA2);
			STH(Reg4, Reg2, 0);
			SetRegister(Reg2, ALT_STAGE_VAL_LOC);
			STH(Reg4, Reg2, 0);
		} EndIf(); EndIf();

		RestoreRegisters();
#if BUILD_TYPE == PROJECT_PLUS
		ASMEnd(0x7cbd2b78); //mr r29, r5
#else
		ASMEnd(0x7fe3fb78); //mr r3, r31
#endif
		//ASMEnd(0x809d0038); //lwz r4, r29, 0x38
	}

	if (CROWD_CHEER_TOGGLE_INDEX != -1) {
		ASMStart(0x8081AD54, "[CM: Code Menu] Crowd Cheer Toggle");
		SaveRegisters();

		int reg1 = 31;

		LoadWordToReg(reg1, CROWD_CHEER_TOGGLE_INDEX + Line::VALUE);
		If(reg1, EQUAL_I, false); {
			RestoreRegisters();
			SetRegister(3, 0);
			BLR();
		} EndIf();

		RestoreRegisters();
		ASMEnd(0x9421ffd0); //stwu sp, -0x30(sp)
	}

	if (STALING_TOGGLE_INDEX != -1) {
		ASMStart(0x808e00a4, "[CM: Code Menu] Staling Toggle");
		
		LoadWordToReg(6, STALING_TOGGLE_INDEX + Line::VALUE);
		If(6, EQUAL_I, 1); {
			SetRegister(0, 8);
		} EndIf();
		If(6, EQUAL_I, 2); {
			SetRegister(0, 0);
		} EndIf();

		SetRegister(6, 0x808E0000);
		ASMEnd(0x5400efff); //rlwinm. r0, r0, 29, 31, 31
	}

	printf("%X\n", SHIELD_RED_1);

	ControlCodes();
}

void CreateMenu(Page MainPage)
{
	applyLineSettingsFromMenuOptionsTree(MainPage, cmnuOptionsOutputFilePath);
	buildMenuOptionsTreeFromMenu(MainPage, cmnuOptionsOutputFilePath);

	//make pages
	CurrentOffset = START_OF_CODE_MENU;
	vector<Page*> Pages(1, &MainPage);
	vector<int> CalledPageOffsets(1, 0);
	int EndOffset = MainPage.Size;
	for (int i = 0; i < Pages.size(); i++) {
		CurrentOffset += Page::NUM_WORD_ELEMS * 4;
		for (Line* &x : Pages[i]->Lines) {
			if (x->Index != nullptr) {
				*(x->Index) = CurrentOffset;
			}
			//printf("%X\n", CurrentOffset);
			CurrentOffset += x->Size;
			if (x->type == PRINT_LINE) {
				int offset = CurrentOffset - x->numArgs * 4;
				//printf("%d\n%X\n%X\n", x->Size, CurrentOffset, offset);
				for (auto arg : x->args) {
					*arg = offset;
					offset += 4;
				}
			}
			if (x->type == SUB_MENU_LINE) {
				x->SubMenuOffset = EndOffset - CalledPageOffsets[i];
				Pages.push_back(x->SubMenuPtr);
				CalledPageOffsets.push_back(EndOffset);
				EndOffset += x->SubMenuPtr->Size;
			}
		}
		//Pages[i]->Lines.back()->Size = 0;
	}

	//setup header
	vector<u8> Header;
	AddValueToByteArray(START_OF_CODE_MENU, Header); //current page ptr
	AddValueToByteArray(START_OF_CODE_MENU, Header); //main page ptr
	//button combos
	AddValueToByteArray(BUTTON_L | BUTTON_R | BUTTON_Y , Header); //salty runback
	AddValueToByteArray(BUTTON_L | BUTTON_R | BUTTON_X, Header); //skip results
	// Old Line Color Table
	AddValueToByteArray(0x00, Header);
	AddValueToByteArray(0x00, Header);
	AddValueToByteArray(0x00, Header);
	AddValueToByteArray(0x00, Header);
	AddValueToByteArray(0x00, Header);
	//frame timers
	AddValueToByteArray(0, Header); //move frame timer
	AddValueToByteArray(0, Header); //value frame timer
	AddValueToByteArray(0, Header); //frame advance frame timer
	//control flags
	AddValueToByteArray(CODE_MENU_CLOSED, Header); //prev flag
	AddValueToByteArray(CODE_MENU_CLOSED, Header); //current flag
	AddValueToByteArray(0, Header); //infinite friendlies flag
	AddValueToByteArray(0, Header); //auto save replays flag
	AddValueToByteArray(0, Header); //group records flag
	//button mask
	AddValueToByteArray(0, Header); //code menu mask
	AddValueToByteArray(0, Header); //button activator mask
	for(int i = 0; i < 8; i++) { AddValueToByteArray(0, Header); } //main mask
	//old debug state
	AddValueToByteArray(0, Header); //old debug state
	AddValueToByteArray(0, Header); //camera lock state
	//old camera pos
	AddValueToByteArray(0, Header); //old camera pos
	//save state buffer ptr
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header); //article ptr
	AddValueToByteArray(0, Header); //article ID ptr
	AddValueToByteArray(0, Header); //article resource ptr
	AddValueToByteArray(0, Header); //locations to update ptr locs
	AddValueToByteArray(0, Header); //locations to clear ptr locs
	AddValueToByteArray(0, Header); //saved article ptr list
	//reset line stack
	AddValueToByteArray(RESET_LINES_STACK_LOC, Header); //reset line stack
	for (int i = 0; i < MAX_SUBPAGE_DEPTH + 1; i++) { AddValueToByteArray(0, Header); }
	//address arrays
	//character switcher
	AddValueToByteArray(CHARACTER_SELECT_P1_INDEX, Header); //P1
	AddValueToByteArray(CHARACTER_SELECT_P2_INDEX, Header); //P2
	AddValueToByteArray(CHARACTER_SELECT_P3_INDEX, Header); //P3
	AddValueToByteArray(CHARACTER_SELECT_P4_INDEX, Header); //P4
	//infinite shields
	AddValueToByteArray(INFINITE_SHIELDS_P1_INDEX, Header); //P1
	AddValueToByteArray(INFINITE_SHIELDS_P2_INDEX, Header); //P2
	AddValueToByteArray(INFINITE_SHIELDS_P3_INDEX, Header); //P3
	AddValueToByteArray(INFINITE_SHIELDS_P4_INDEX, Header); //P4
	//percent selection values
	AddValueToByteArray(PERCENT_SELECT_VALUE_P1_INDEX, Header); //P1
	AddValueToByteArray(PERCENT_SELECT_VALUE_P2_INDEX, Header); //P2
	AddValueToByteArray(PERCENT_SELECT_VALUE_P3_INDEX, Header); //P3
	AddValueToByteArray(PERCENT_SELECT_VALUE_P4_INDEX, Header); //P4
	//percent selection activators
	AddValueToByteArray(PERCENT_SELECT_ACTIVATOR_P1_INDEX, Header); //P1
	AddValueToByteArray(PERCENT_SELECT_ACTIVATOR_P2_INDEX, Header); //P2
	AddValueToByteArray(PERCENT_SELECT_ACTIVATOR_P3_INDEX, Header); //P3
	AddValueToByteArray(PERCENT_SELECT_ACTIVATOR_P4_INDEX, Header); //P4
	//disable DPad activators
	AddValueToByteArray(DISABLE_DPAD_P1_INDEX, Header); //P1
	AddValueToByteArray(DISABLE_DPAD_P2_INDEX, Header); //P2
	AddValueToByteArray(DISABLE_DPAD_P3_INDEX, Header); //P3
	AddValueToByteArray(DISABLE_DPAD_P4_INDEX, Header); //P4
	//Endless Rotation player queue
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	//Endless Rotation sort buffer
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	//Endless Rotation comp func
	AddValueToByteArray(0x80630000, Header);
	AddValueToByteArray(0x80840000, Header);
	AddValueToByteArray(0x7C641850, Header);
	AddValueToByteArray(0x4E800020, Header);
	//replay buffers
	for(int i = 0; i < 5; i++) { AddValueToByteArray(0, Header); } //nte buffer
	for(int i = 0; i < 2; i++) { AddValueToByteArray(0, Header); } //section buffer
	for(int i = 0; i < 12; i++) { AddValueToByteArray(0, Header); } //crypto buffer
	//button conversion tables
	Header.insert(Header.end(), CODE_MENU_WIIMOTE_CONVERSION_TABLE.begin(), CODE_MENU_WIIMOTE_CONVERSION_TABLE.end());
	Header.insert(Header.end(), CODE_MENU_WIICHUCK_CONVERSION_TABLE.begin(), CODE_MENU_WIICHUCK_CONVERSION_TABLE.end());
	Header.insert(Header.end(), CODE_MENU_CLASSIC_CONVERSION_TABLE.begin(), CODE_MENU_CLASSIC_CONVERSION_TABLE.end());
	//Display tag hex
	for (auto x : { P1_TAG_HEX_LOC, P2_TAG_HEX_LOC, P3_TAG_HEX_LOC, P4_TAG_HEX_LOC }) {
		AddValueToByteArray('$', Header);
		for (int i = 0; i < 23; i++) { AddValueToByteArray((u8)0, Header); }
	}
	//Endless Friendlies rotation ptrs
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	AddValueToByteArray(0, Header);
	
	//random alts
	//rng
	AddValueToByteArray(0, Header);
	//start match flag
	AddValueToByteArray(1, Header);

	//TEAM_SETTINGS_LOC
	AddValueToByteArray(0, Header);
	//TAG_LOAD_FLAGS_LOC
	AddValueToByteArray(0, Header);

	//PREV_TAG_COSTUMES_SETTING_LOC
	AddValueToByteArray(0, Header);

	//DOLPHIN_MOUNT_VF_LOC
	AddValueToByteArray(0, Header);

	//CODE_MENU_OLD_CAMERA_MATRIX_LOC
	for (int i = 0; i < 12; i++) { AddValueToByteArray(0, Header); }
	//CODE_MENU_NEED_TO_SAVE_CAMERA_MATRIX_FLAG_LOC
	AddValueToByteArray(0, Header);

	//SHOULD_DISPLAY_HUD_FLAG_LOC
	AddValueToByteArray(0, Header);

	//SHOULD_RESET_HITBOX_DISPLAY_FLAG_LOC
	AddValueToByteArray(0, Header);
	//SHOULD_RESET_STAGE_COLLISIONS_FLAG_LOC
	AddValueToByteArray(0, Header);

	//Auto L-Cancelling
	AddValueToByteArray(ALC_P1_INDEX, Header);
	AddValueToByteArray(ALC_P2_INDEX, Header);
	AddValueToByteArray(ALC_P3_INDEX, Header);
	AddValueToByteArray(ALC_P4_INDEX, Header);
	
	//Big Head Mode Index
	AddValueToByteArray(BIG_HEAD_INDEX, Header);

	//Random Angle Mode
	AddValueToByteArray(RANDOM_ANGLE_INDEX, Header);

	//War Mode Index
	AddValueToByteArray(WAR_MODE_INDEX, Header);

	//Input Buffer
	AddValueToByteArray(BUFFER_P1_INDEX, Header);
	AddValueToByteArray(BUFFER_P2_INDEX, Header);
	AddValueToByteArray(BUFFER_P3_INDEX, Header);
	AddValueToByteArray(BUFFER_P4_INDEX, Header);

	//Scale Modifier
	AddValueToByteArray(SCALE_INDEX, Header);

	//Scale Modifier
	AddValueToByteArray(SPEED_INDEX, Header);

	// CSS VER Modifier
	AddValueToByteArray(CSS_VERSION_SETTING_INDEX, Header);

	// Theme Setting
	AddValueToByteArray(THEME_SETTING_INDEX, Header);

	// Dash Attack Item Grab Setting
	AddValueToByteArray(DASH_ATTACK_ITEM_GRAB_INDEX, Header);
	
	// Tripping Toggle
	AddValueToByteArray(TRIP_TOGGLE_INDEX, Header);
	// Tripping Rate Multiplier
	AddValueToByteArray(TRIP_RATE_MULTIPLIER_INDEX, Header);
	// Tripping Cooldown Toggle
	AddValueToByteArray(TRIP_INTERVAL_INDEX, Header);

	// Backplate Settings
	AddValueToByteArray(BACKPLATE_COLOR_1_INDEX, Header);
	AddValueToByteArray(BACKPLATE_COLOR_2_INDEX, Header);
	AddValueToByteArray(BACKPLATE_COLOR_3_INDEX, Header);
	AddValueToByteArray(BACKPLATE_COLOR_4_INDEX, Header);
	AddValueToByteArray(BACKPLATE_COLOR_C_INDEX, Header);
	AddValueToByteArray(BACKPLATE_COLOR_T_INDEX, Header);
	//BACKPLATE_COLOR_TEAM_BATTLE_STORE_LOC
	// Used to store some temp values related to the color changer!
	// First byte is an offset used to lbzx to either VALUE or DEFAULT quickly (init to VALUE).
	AddValueToByteArray(Line::VALUE << 0x18, Header);

	// Jumpsquat Override
	AddValueToByteArray(JUMPSQUAT_OVERRIDE_TOGGLE_INDEX, Header);
	AddValueToByteArray(JUMPSQUAT_OVERRIDE_FRAMES_INDEX, Header);
	AddValueToByteArray(JUMPSQUAT_OVERRIDE_MIN_INDEX, Header);
	AddValueToByteArray(JUMPSQUAT_OVERRIDE_MAX_INDEX, Header);
	
	//draw settings buffer
	vector<u32> DSB(0x200 / 4, 0);
	DSB[0x4 / 4] = 0xFFFFFFFF;
	DSB[0x38 / 4] = 1;
	DSB[0x3C / 4] = 1;
	DSB[0x4C / 4] = 0x01000000;
	DSB[0x50 / 4] = 0x3D800000;
	DSB[0x58 / 4] = 0x3f793cde;
	DSB[(0x100 + 0x4C) / 4] = DRAW_SETTINGS_BUFFER_LOC + 0x100;
	DSB[0x1D0 / 4] = DRAW_SETTINGS_BUFFER_LOC + 0x100;
	for (auto x : DSB) {
		AddValueToByteArray(x, Header);
	}

	// If there are MEM2Constants...
	if (MEM2_CONSTANTS_LENGTH > 0)
	{
		// ... reserve space for them in the CMNU!
		Header.resize(Header.size() + MEM2_CONSTANTS_LENGTH, 0xCC);
	}

	if (LINE_COLOR_TABLE.table_size() > 0)
	{
		for (std::size_t i = 0; i < LINE_COLOR_TABLE.__COLOR_COUNT; i++)
		{
			AddValueToByteArray(LINE_COLOR_TABLE.COLORS[i], Header);
		}
	}

	if (HEAP_ADDRESS_TABLE.table_size() > 0)
	{
		// Reserve space for Address Table (just initialized to zeroes, value populated in-game).
		Header.resize(Header.size() + HEAP_ADDRESS_TABLE.address_array_size(), 0x00);
		// Write ID Array to Header.
		Header.insert(Header.end(), HEAP_ADDRESS_TABLE.idArray.cbegin(), HEAP_ADDRESS_TABLE.idArray.cend());
	}

	if (START_OF_CODE_MENU - START_OF_CODE_MENU_HEADER != Header.size()) {
		cout << "Messed up header\n";
		exit(-1);
	}

	copy(Header.begin(), Header.end(), ostreambuf_iterator<char>(MenuFile));

	for (auto x : Pages) {
		x->WritePage();
	}
}

void constantOverride() {
	ASMStart(0x80023d60, "[CM: Code Menu] Constant Overrides");

	int reg1 = 4;
	int reg2 = 5;

	for(auto& x : constantOverrides) {
		LoadWordToReg(reg1, *x.index + Line::VALUE);
		SetRegister(reg2, x.address);
		STW(reg1, reg2, 0);
	}

	ASMEnd(0x2c000000); //cmpwi, r0, 0
}

void ControlCodeMenu()
{
	ASMStart(0x80029574, "[CM: Code Menu] Control Code Menu");
	vector<int> FPRegs(14);
	iota(FPRegs.begin(), FPRegs.end(), 0);
	SaveRegisters(FPRegs);

	if (SHIELD_RED_1 != -1) {
		SetRegister(5, 0x80f574c0 + 0x88);
		SetRegister(3, SHIELD_RED_1 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 0);
		SetRegister(3, SHIELD_GREEN_1 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 1);
		SetRegister(3, SHIELD_BLUE_1 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 2);
		SetRegister(3, SHIELD_ALPHA_1 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 3);
		SetRegister(5, 0x80f574c0 + 0x90);
		SetRegister(3, SHIELD_RED_2 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 0);
		SetRegister(3, SHIELD_GREEN_2 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 1);
		SetRegister(3, SHIELD_BLUE_2 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 2);
		SetRegister(3, SHIELD_ALPHA_2 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 3);
		SetRegister(5, 0x80f574c0 + 0xA0);
		SetRegister(3, SHIELD_RED_3 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 0);
		SetRegister(3, SHIELD_GREEN_3 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 1);
		SetRegister(3, SHIELD_BLUE_3 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 2);
		SetRegister(3, SHIELD_ALPHA_3 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 3);
		SetRegister(5, 0x80f574c0 + 0xA8);
		SetRegister(3, SHIELD_RED_4 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 0);
		SetRegister(3, SHIELD_GREEN_4 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 1);
		SetRegister(3, SHIELD_BLUE_4 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 2);
		SetRegister(3, SHIELD_ALPHA_4 + 8 + 3);
		LBZ(4, 3, 0);
		STB(4, 5, 3);
	}



	printMenuSetters();

	int Reg1 = 31;
	int Reg2 = 30;
	int Reg3 = 29;
	int Reg4 = 28;
	int Reg5 = 27;
	int Reg6 = 26;
	int Reg7 = 25;
	int Reg8 = 24;
	int Reg9 = 23;
	int CharacterBufferReg = 21;
	int MainBufferReg = 20;
	int OpenFlagReg = 19;
	int ControlStickXReg = 18;
	int ControlStickYReg = 17;
	int CounterReg = 16;
	int ButtonReg = 15;
	int ActionReg = 14;

	int NotLoaded = GetNextLabel();

	// Rewritten check for whether or not menu is loaded!
	// Load the main page pointer from just past the Menu Header Loc into Reg4 (this should be START_OF_CODE_MENU).
	ADDIS(Reg1, 0, START_OF_CODE_MENU_HEADER >> 0x10);
	LWZ(Reg4, Reg1, (START_OF_CODE_MENU_HEADER & 0xFFFF) + 4);
	// Add the bottom half of START_OF_CODE_MENU to Reg1, so Reg1 should *also* now be START_OF_CODE_MENU.
	ADDI(Reg1, Reg1, START_OF_CODE_MENU & 0xFFFF);
	// Compare the two as unsigned integers.
	CMPL(Reg1, Reg4, EQUAL_L);
	// And if the two aren't equal, then we know the menu isn't loaded, skip to notLoaded tag!
	JumpToLabel(NotLoaded, bCACB_NOT_EQUAL);

	LoadWordToReg(Reg4, HUD_DISPLAY_INDEX + Line::VALUE);
	If(Reg4, EQUAL_I, 0);
	{
		SetRegister(Reg4, SHOULD_DISPLAY_HUD_FLAG_LOC);
		SetRegister(Reg1, 1);
		STW(Reg1, Reg4, 0);
		
		//Remove HUD
		SetRegister(3, 0x80672f40);
		SetRegister(4, 8);
		SetRegister(5, 0);
		CallBrawlFunc(0x8000D234); //setLayerDispStatus[gfSceneRoot]
	} Else();
	{
		LoadWordToReg(Reg1, Reg4, SHOULD_DISPLAY_HUD_FLAG_LOC);
		If(Reg1, EQUAL_I, 1);
		{
			//Show HUD
			SetRegister(3, 0x80672f40);
			SetRegister(4, 8);
			SetRegister(5, 1);
			CallBrawlFunc(0x8000D234); //setLayerDispStatus[gfSceneRoot]

			SetRegister(Reg1, 0);
			STW(Reg1, Reg4, 0);
		} EndIf();
	} EndIf();
	
	
	LoadWordToReg(OpenFlagReg, Reg4, CODE_MENU_CONTROL_FLAG);

	//GCC input
	SetRegister(Reg1, PLAY_BUTTON_LOC_START - BUTTON_PORT_OFFSET);
	SetRegister(Reg7, 0x804DE4B0 - 8);
	SetRegister(ButtonReg, 0);
	SetRegister(ControlStickXReg, 0);
	SetRegister(ControlStickYReg, 0);
	CounterLoop(CounterReg, 0, 4, 1); {
		LHZU(Reg8, Reg7, 8);
		LWZU(Reg2, Reg1, BUTTON_PORT_OFFSET);
		If(Reg8, GREATER_OR_EQUAL_I_L, 0x2000);
		{
			SetRegister(Reg8, 0);
		}EndIf();
		ANDI(Reg8, Reg8, 0x1000);
		OR(ButtonReg, ButtonReg, Reg2);
		OR(ButtonReg, ButtonReg, Reg8);

		ANDI(Reg8, Reg2, BUTTON_L | BUTTON_R | BUTTON_DD);
		If(Reg8, GREATER_I, BUTTON_L | BUTTON_R); {
			//L + R + DDown is pressed
			If(OpenFlagReg, NOT_EQUAL_I, CODE_MENU_OPEN); {
				SetRegister(OpenFlagReg, CODE_MENU_TRIGGERED);
			}EndIf();
		}EndIf();
		
		LBA(Reg2, Reg1, GCC_CONTROL_STICK_X_START - PLAY_BUTTON_LOC_START);
		ADD(ControlStickXReg, ControlStickXReg, Reg2);

		LBA(Reg2, Reg1, GCC_CONTROL_STICK_Y_START - PLAY_BUTTON_LOC_START);
		ADD(ControlStickYReg, ControlStickYReg, Reg2);
	}CounterLoopEnd();

	//Wiimote control stick
	CounterLoop(CounterReg, 0, 4, 1); {
		ADDI(Reg1, Reg1, BUTTON_PORT_OFFSET);

		LBA(Reg2, Reg1, GCC_CONTROL_STICK_X_START - PLAY_BUTTON_LOC_START);
		ADD(ControlStickXReg, ControlStickXReg, Reg2);

		LBA(Reg2, Reg1, GCC_CONTROL_STICK_Y_START - PLAY_BUTTON_LOC_START);
		ADD(ControlStickYReg, ControlStickYReg, Reg2);
	}CounterLoopEnd();

	//Wiimote input
	SetRegister(Reg1, WII_BASED_CONTROLLER_START - WII_BASED_CONTROLLER_PORT_OFFSET);
	SetRegister(Reg3, 1);
	CounterLoop(Reg8, 0, 4, 1); {
		//Reg8 is loop counter, Reg3 is 1, Reg1 is wiimote location, Reg2 is storage loc, Reg9 is activation address, Reg6 is conversion
		//Reg5 has wiimote buttons
		LHZU(Reg5, Reg1, WII_BASED_CONTROLLER_PORT_OFFSET); //get buttons
		SetRegister(Reg6, CODE_MENU_WIIMOTE_CONVERSION_TABLE_LOC);
		LBZ(Reg7, Reg1, WII_BASED_CONTROLLER_TYPE_OFFSET); //get type
		If(Reg7, EQUAL_I, 2); {
			//is classic
			LHZ(Reg5, Reg1, WII_BASED_CONTROLLER_TYPE_OFFSET + 2); //buttons are in different place
			ANDI(Reg9, Reg5, 0xC003); //get if DPad pressed
			If(Reg9, NOT_EQUAL_I, 0); {
				ANDI(Reg9, Reg5, 0x2200);
				If(Reg9, EQUAL_I, 0x2200); {
					If(OpenFlagReg, NOT_EQUAL_I, CODE_MENU_OPEN); {
						SetRegister(OpenFlagReg, CODE_MENU_TRIGGERED);
					}EndIf();
				}EndIf();
			}EndIf();
		}Else(); {
			ANDI(Reg9, Reg5, 0x1900);
			If(Reg9, EQUAL_I, 0x1900); {
				If(OpenFlagReg, NOT_EQUAL_I, CODE_MENU_OPEN); {
					SetRegister(OpenFlagReg, CODE_MENU_TRIGGERED);
				}EndIf();
			}EndIf();
		}EndIf();
		MULLI(Reg7, Reg7, 16);
		ADD(Reg6, Reg6, Reg7); //get proper conversion table

		While(Reg5, GREATER_I, 0); {
			//convert buttons
			ANDI(Reg7, Reg5, 1);
			If(Reg7, NOT_EQUAL_I, 0); {
				//button pressed
				LBZ(Reg7, Reg6, 0);
				RLWNM(Reg7, Reg3, Reg7, 15, 31);
				OR(ButtonReg, ButtonReg, Reg7);
			}EndIf();

			ADDI(Reg6, Reg6, 1);
			RLWINM(Reg5, Reg5, 31, 15, 31);
		}EndWhile();
	}CounterLoopEnd();


	LoadWordToReg(Reg3, Reg5, IS_DEBUG_PAUSED);
	
	If(OpenFlagReg, EQUAL_I, CODE_MENU_PRIMED); {
		//check for A press
		SetRegister(OpenFlagReg, CODE_MENU_CLOSED);

		ANDI(Reg8, ButtonReg, BUTTON_A | BUTTON_START);
		If(Reg8, NOT_EQUAL_I, 0); {
			//A or start is pressed
			SetRegister(OpenFlagReg, CODE_MENU_TRIGGERED); //set open
		}EndIf();
	}EndIf();

	if (CODE_MENU_ACTIVATION_SETTING_INDEX != -1) {
		LoadWordToReg(Reg2, IS_IN_GAME_FLAG);
		If(Reg2, EQUAL_I, 1); {
			LoadWordToReg(Reg2, CODE_MENU_ACTIVATION_SETTING_INDEX + Line::VALUE);
			If(Reg2, NOT_EQUAL_I, 0); {
				LoadWordToReg(Reg2, CODE_MENU_CONTROL_FLAG);
				If(Reg2, EQUAL_I, CODE_MENU_CLOSED); {
					SetRegister(OpenFlagReg, CODE_MENU_CLOSED);
				}EndIf();
			}EndIf();
		}EndIf();
	}

	If(OpenFlagReg, EQUAL_I, CODE_MENU_TRIGGERED); {
		SetRegister(Reg2, OLD_DEBUG_STATE_LOC);
		SetRegister(OpenFlagReg, CODE_MENU_OPEN); //set open
		STW(Reg3, Reg2, 0); //store debug state

		//save and set camera pos
		SetRegister(Reg3, OLD_CAMERA_POS_LOC);
		LoadWordToReg(Reg1, 0x805b6df8);
		STW(Reg1, Reg3, 0);

		SetRegister(Reg2, CODE_MENU_BUTTON_MASK_LOC);
		STW(ButtonReg, Reg2, 0);
	}EndIf();

	//get only pressed button
	LoadWordToReg(Reg2, Reg3, CODE_MENU_BUTTON_MASK_LOC);
	ORI(Reg9, Reg2, ~(BUTTON_Z | BUTTON_DPAD));
	AND(Reg9, ButtonReg, Reg9);
	STW(Reg9, Reg3, 0); //update mask
	ANDC(ButtonReg, ButtonReg, Reg2);

	STW(OpenFlagReg, Reg4, 0); //set flag

	If(OpenFlagReg, EQUAL_I, CODE_MENU_OPEN); {
		//prevent buttons from affecting the game
		SetRegister(Reg2, 0xFFFFFFFF);
		SetRegister(Reg3, MAIN_BUTTON_MASK_LOC - 4);
		CounterLoop(CounterReg, 0, 8, 1); {
			STWU(Reg2, Reg3, 4);
		}CounterLoopEnd();

		//stop stick inputs
		SetRegister(Reg1, GCC_CONTROL_STICK_X_START - BUTTON_PORT_OFFSET);
		SetRegister(Reg2, 0);
		CounterLoop(CounterReg, 0, 8, 1); {
			STHU(Reg2, Reg1, BUTTON_PORT_OFFSET); //clear stick input
		}CounterLoopEnd();

		SetRegister(Reg1, 1);
		STW(Reg1, Reg5, 0); //set debug paused

		GetActionFromInputs(ButtonReg, ControlStickXReg, ControlStickYReg, ActionReg);

		ExecuteAction(ActionReg);

	}EndIf(); //run logic
	
	//button negate
	SetRegister(Reg1, PLAY_BUTTON_LOC_START - BUTTON_PORT_OFFSET);
	SetRegister(Reg2, MAIN_BUTTON_MASK_LOC - 4);
	CounterLoop(CounterReg, 0, 8, 1); {
		LWZU(Reg3, Reg2, 4); //mask
		LWZU(Reg4, Reg1, BUTTON_PORT_OFFSET); //buttons

		AND(Reg5, Reg4, Reg3);
		STW(Reg5, Reg2, 0); //update mask
		ANDC(Reg4, Reg4, Reg3); //apply mask
		STW(Reg4, Reg1, 0); //store new buttons
	}CounterLoopEnd();

	//LoadWordToReg(Reg6, ON_GROUP_RECORDS_FLAG_LOC);
	SetRegister(Reg6, 0);
	If(Reg6, EQUAL_I, 1); {
		SetRegister(Reg1, PLAY_BUTTON_LOC_START - BUTTON_PORT_OFFSET);
		SetRegister(Reg2, ~(BUTTON_A | BUTTON_START));
		CounterLoop(CounterReg, 0, 8, 1); {
			LWZU(Reg4, Reg1, BUTTON_PORT_OFFSET); //buttons
			AND(Reg4, Reg4, Reg2);
			STW(Reg4, Reg1, 0); //store new buttons
		}CounterLoopEnd();
	}EndIf();

	int SkipDebugNegation = GetNextLabel();

	if (CODE_MENU_ACTIVATION_SETTING_INDEX != -1) {
		LoadWordToReg(Reg2, IS_IN_GAME_FLAG);
		If(Reg2, EQUAL_I, 1); {
			LoadWordToReg(Reg2, CODE_MENU_ACTIVATION_SETTING_INDEX + Line::VALUE);
			If(Reg2, EQUAL_I, 1); {
				LoadWordToReg(Reg2, CODE_MENU_CONTROL_FLAG);
				If(Reg2, EQUAL_I, CODE_MENU_CLOSED); {
					JumpToLabel(SkipDebugNegation);
				}EndIf();
			}EndIf();
		}EndIf();
	}

	LoadWordToReg(Reg1, Reg3, 0x80584000);
	LoadWordToReg(Reg2, CODE_MENU_CONTROL_FLAG);
	ANDI(Reg6, ButtonReg, BUTTON_Z);
	If(Reg2, EQUAL_I, CODE_MENU_OPEN); // prevent z frame advance in menu
	{
		SetRegister(Reg6, 0);
	} EndIf();
	LoadWordToReg(Reg4, Reg5, FRAME_ADVANCE_FRAME_TIMER);
	SetRegister(Reg2, ~(0x10000000 | 0x00100000)); //stop all debug commands except pause and frame advance
	If(Reg6, NOT_EQUAL_I, 0); {
		Decrement(Reg4);
		If(Reg4, LESS_OR_EQUAL_I, 0); {
			If(Reg4, LESS_I, 0); {
				SetRegister(Reg4, FIRST_FRAME_ADVANCE_NUM_WAIT_FRAMES - FRAME_ADVANCE_NUM_WAIT_FRAMES);
			}EndIf();
			ANDIS(Reg1, Reg1, ~0x0010); //allow frame advance
			ADDI(Reg4, Reg4, FRAME_ADVANCE_NUM_WAIT_FRAMES);

			CounterLoop(Reg7, 0, 32, 8); //The actual suggested fix
			{
				LHZX(Reg8, Reg3, Reg7);
				ANDI(Reg8, Reg8, ~0x0010);
				STHX(Reg8, Reg3, Reg7);
			} CounterLoopEnd();
		}EndIf();
	}Else(); {
		SetRegister(Reg4, 0);
	}EndIf();
	STW(Reg4, Reg5, 0);

	If(OpenFlagReg, EQUAL_I, CODE_MENU_OPEN); {
		SetRegister(Reg2, -1); //stop all debug commands
	}EndIf();

	//Eon's Project+ 2.28 Fix
	SetRegister(Reg4, 0);
	While(Reg4, NOT_EQUAL_I, 32);
	{
		LWZX(Reg1, Reg3, Reg4);
		OR(Reg1, Reg2, Reg1);
		STWX(Reg1, Reg3, Reg4);
		ADDI(Reg4, Reg4, 0x8);
	}
	EndWhile();
	//Lines that were removed by Eon's P+ 2.28 Fix. 
	//OR(Reg1, Reg1, Reg2);
	//STW(Reg1, Reg3, 0);
	//stop Z and start if in menu

	ApplyMenuSetting(DEBUG_MODE_INDEX, 0x80583FFC + 3, Reg1, Reg2, 1);

	ApplyMenuSetting(DISPLAY_HITBOXES_INDEX, 0x80583FFC + 1, Reg1, Reg2, 1);

	ApplyMenuSetting(DISPLAY_LEDGEGRAB_INDEX, 0x80583FF8 + 1, Reg1, Reg2, 1);

	ApplyMenuSetting(DISPLAY_COLLISION_INDEX, 0x80583FF4 + 3, Reg1, Reg2, 1);

	ApplyMenuSetting(CAMERA_LOCK_INDEX, 0x80583FF8 + 3, Reg1, Reg2, 1);

	if (RANDOM_1_TO_1_INDEX != -1) {
		ApplyMenuSetting(RANDOM_1_TO_1_INDEX, RANDOM_1_TO_1_CPP_FLAG_LOC, Reg1, Reg2, 1);
		printf("1 to 1 location %0X\n", RANDOM_1_TO_1_CPP_FLAG_LOC);
	}

	Label(SkipDebugNegation);

	//dumb solution to code menu closing problem
	LoadWordToReg(OpenFlagReg, CODE_MENU_CONTROL_FLAG);

	if (CODE_MENU_ACTIVATION_SETTING_INDEX != -1) {
		If(OpenFlagReg, EQUAL_I, CODE_MENU_CLOSING); {
			LoadWordToReg(Reg1, CODE_MENU_ACTIVATION_SETTING_INDEX + Line::VALUE);
			If(Reg1, EQUAL_I, 2); {
				//save certain settings
				SetRegister(Reg7, CODE_MENU_ACTIVATION_SETTING_INDEX);
				SetRegister(Reg8, AUTO_SAVE_REPLAY_INDEX);
				SetRegister(Reg9, AUTO_SKIP_TO_CSS_INDEX);
				LWZ(8, Reg7, Line::VALUE);
				LBZ(9, Reg7, Line::COLOR);
				LWZ(4, Reg8, Line::VALUE);
				LBZ(5, Reg8, Line::COLOR);
				LWZ(6, Reg9, Line::VALUE);
				LBZ(7, Reg9, Line::COLOR);

				LoadWordToReg(Reg2, MAIN_PAGE_PTR_LOC);
				SetRegister(Reg1, RESET_LINES_STACK_LOC);
				PushOnStack(Reg2, Reg1);
				ResetPage(Reg1, Reg2, Reg3, Reg4, Reg5, Reg6, 3);

				//restore certain settings
				STW(8, Reg7, Line::VALUE);
				STB(9, Reg7, Line::COLOR);
				STW(4, Reg8, Line::VALUE);
				STB(5, Reg8, Line::COLOR);
				STW(6, Reg9, Line::VALUE);
				STB(7, Reg9, Line::COLOR);
			}EndIf();
		}EndIf();
	}

	//port based codes
	LoadWordToReg(Reg7, IS_IN_GAME_FLAG);
	If(OpenFlagReg, NOT_EQUAL_I, CODE_MENU_OPEN); {
		If(Reg7, EQUAL_I, 1); {
			//in match
			LoadWordToReg(MainBufferReg, MAIN_BUFFER_PTR);
			LWZU(CharacterBufferReg, MainBufferReg, 4);
			While(CharacterBufferReg, NOT_EQUAL_I, 0); {
				LWZ(Reg8, CharacterBufferReg, CHR_BUFFER_PORT_OFFSET);

				if (DISABLE_DPAD_P1_INDEX != -1) {
					LWZ(Reg2, CharacterBufferReg, CHR_BUFFER_INFO_PTR_OFFSET);
					RunIfPortToggle(DISABLE_DPAD_ACTIVATOR_ARRAY_LOC, Reg8); {
						LBZ(Reg2, Reg2, 7); //controller num
						SetRegister(Reg3, PLAY_BUTTON_LOC_START - 0x40);
						MULLI(Reg2, Reg2, BUTTON_PORT_OFFSET);
						LWZUX(Reg2, Reg3, Reg2); //get buttons
						ANDI(Reg2, Reg2, ~BUTTON_DPAD);
						STW(Reg2, Reg3, 0);
					}EndIf(); EndIf(); EndIf();
				}

				if (PERCENT_SELECT_ACTIVATOR_P1_INDEX != -1 && PERCENT_SELECT_VALUE_P1_INDEX != -1) {
					GetArrayValueFromIndex(PERCENT_SELCTION_ACTIVATOR_ARRAY_LOC, Reg8, 0, 3); {
						LWZ(5, 3, Line::VALUE); //get setting
						LWZ(Reg2, CharacterBufferReg, CHR_BUFFER_INFO_PTR_OFFSET);
						If(5, EQUAL_I, 1); {
							ANDI(Reg1, ButtonReg, 0xF);
							If(Reg1, NOT_EQUAL_I, 0); {
								GetArrayValueFromIndex(PERCENT_SELCTION_VALUE_ARRAY_LOC, Reg8, 0, 3); {
									LFS(1, 3, Line::VALUE);

									SetRegister(Reg3, 0x80615520);
									RLWINM(Reg4, Reg8, 2, 0, 31); //<< 2
									ADD(Reg3, Reg3, Reg4);	//Reg8 changed to Reg4, P+ 2.28 fix
									LWZ(Reg3, Reg3, 0x4C);

									FCTIWZ(0, 1);
									STFD(0, 1, -0x30);
									LWZ(Reg4, 1, -0x2C);
									STW(Reg4, Reg3, 0x18);
									ADDI(Reg1, Reg4, 1);

									STW(Reg1, Reg3, 0x1C);
									LWZ(Reg3, CharacterBufferReg, 0x3C);
									LWZ(Reg3, Reg3, 0x60);
									LWZ(Reg3, Reg3, 0xD8);
									LWZ(Reg3, Reg3, 0x38);
									LWZ(Reg3, Reg3, 0x40);
									STFS(1, Reg3, 0xC);

									LWZ(3, CharacterBufferReg, CHR_BUFFER_HEAD_OF_FIGHTER_OFFSET);
									CallBrawlFunc(0x8083ae24); //getOwner
									LWZ(3, 3, 0);
									STFS(1, 3, 0x24);


									
									/*LWZ(3, CharacterBufferReg, CHR_BUFFER_HEAD_OF_FIGHTER_OFFSET);
									CallBrawlFunc(0x8083ae24); //getOwner
									//SetRegister(4, 1);
									ConvertFloatingRegToInt(1, Reg1, 0);
									CallBrawlFunc(0x8081bdcc); //set damage

									LoadWordToReg(Reg3, 0x805A02D0);
									LBZ(Reg4, Reg2, 3); //player num
									MULLI(Reg4, Reg4, 4);
									ADD(Reg3, Reg3, Reg4);
									LWZ(Reg3, Reg3, 0x4C);

									ADDI(4, Reg1, 1);
									If(Reg1, GREATER_I, 999); {
										SetRegister(4, 998);
									}EndIf();
									MR(3, Reg3);
									SetRegister(5, 0);
									CallBrawlFunc(0x800e14a4); //updateDamageHP

									MR(4, Reg1);
									MR(3, Reg3);
									SetRegister(5, 1);
									CallBrawlFunc(0x800e14a4); //updateDamageHP*/


									
								}EndIf(); EndIf();
							}EndIf();
						}EndIf();
					}EndIf(); EndIf();
				}

				if (CHARACTER_SELECT_P1_INDEX != -1) {
					If(OpenFlagReg, EQUAL_I, CODE_MENU_CLOSING); {
						GetArrayValueFromIndex(CHARACTER_SWITCHER_ARRAY_LOC, Reg8, 0, 3); {
							LWZ(5, 3, Line::VALUE); //get setting
							STW(5, 3, Line::DEFAULT);
							LWZ(Reg2, CharacterBufferReg, CHR_BUFFER_INFO_PTR_OFFSET);
							RLWINM(5, 5, 2, 0, 31); //<<2
							LBZ(4, Reg2, 0); //get current ID
							ADDI(5, 5, Selection::SELECTION_LINE_OFFSETS_START + 2);
							LWZ(Reg1, 3, Selection::SELECTION_LINE_SOURCE_SELECTION_INDEX);
							LHZX(5, Reg1, 5);
							If(4, NOT_EQUAL, 5); { // If the New ID and Old ID don't match...
								SetRegister(Reg1, 0);
								STB(5, Reg2, 0); // Overwrite Old ID with New one
								// This seems to trigger a line at 0x809463C4 in "processBegin/[stLoaderPlayer]/st_loader_player.o" (0x80954350 in Ghidra)
								// which ultimately leads to the line:
								//		(*(code *)this->vtable->stLoaderPlayer$$removeEntity)(this); Starting @ 0x809543d4 in Ghidra
								// This, I assume, is the thing that kills the entity and prompts spawning the new one.
								// There's *also*, however, 0x80946358 (Ghidra: 0x809542e4), which directly checks whether or not the Slot ID
								// recorded in the PlayerInitStruct is different from the currently selected one, along with some other conditions.
								// If so, then we run the following:
								//		(*(code *)this->vtable->stLoaderPlayer$$removeEntity)(this); Starting @ 0x8095431c in Ghidra
								// Either one of these could be the thing responsible for ultimately causing the change to happen,
								// not sure which it is for sure.
								// In either case, should end up calling "removeEntity/[stLoaderPlayer]" 0x80948da8 (0x80956d34 in Ghidra)
								// Messing with the r3 value at 0x809463C4 does prompt a reload
								// Messing with the r0 value at 0x80946358 DOES NOT! Neither in SSE, nor in VS

								SetRegister(Reg3, 0x43AD8);
								LoadWordToReg(Reg4, 0x805A00E0); // Get ptr to GameGlobal Struct
								LWZ(Reg4, Reg4, 0x10); // Get ptr to gmSelCharData (0x90180b40 in testing)
								ADD(Reg4, Reg4, Reg3); // Add 0x43AD8 to addr of this struct (901c4618)? Way beyond the listed range in Ghidra
								LWZ(Reg3, CharacterBufferReg, CHR_BUFFER_PORT_OFFSET); // Get port for character to change...
								MULLI(Reg3, Reg3, 0x5C); // ... and multiply it by 0x5C, probably to index into a list of entries
								STWX(Reg1, Reg4, Reg3); // Write 0 into (&gmSelCharData + 0x43AD8 + OffsetIntoListForTargetPort), Setting to 1 gives RAlt?

								STB(Reg1, Reg2, 5); //force costume to 0
								STB(Reg1, Reg2, 6); //force HUD to 0
							}EndIf();
						}EndIf(); EndIf();
					}EndIf();
				}

				if (INFINITE_SHIELDS_P1_INDEX != -1) {
					GetArrayValueFromIndex(INIFINITE_SHIELDS_ARRAY_LOC, Reg8, 0, 3); {
						LWZ(3, 3, Line::VALUE);
						If(3, EQUAL_I, 1); {
							LWZ(Reg1, CharacterBufferReg, CHR_BUFFER_VARIABLES_ADDRESS_OFFSET);
							SetRegister(Reg2, 0x42700000);
							STW(Reg2, Reg1, 0x19C);
						}EndIf();
					}EndIf(); EndIf();
				}

				LWZU(CharacterBufferReg, MainBufferReg, 8);
			}EndWhile();
		}EndIf();
	}EndIf();

	If(OpenFlagReg, EQUAL_I, CODE_MENU_CLOSING); {
		LoadWordToReg(3, 4, PREV_CODE_MENU_CONTROL_FLAG);
		STW(3, 4, CODE_MENU_CONTROL_FLAG - PREV_CODE_MENU_CONTROL_FLAG);
	}EndIf();

	if (AUTO_SAVE_REPLAY_INDEX != -1) {
		LoadWordToReg(Reg3, IS_REPLAY_LOC);
		SetRegister(Reg2, AUTO_SAVE_REPLAY_FLAG_LOC);
		If(Reg3, EQUAL_I, 2); {
			STW(Reg3, Reg2, 0); //set flag
		}EndIf();
		LoadWordToReg(Reg1, AUTO_SAVE_REPLAY_INDEX + Line::VALUE);
		If(Reg1, NOT_EQUAL_I, 1); {
			SetRegister(Reg1, 0);
			STW(Reg1, Reg2, 0); //clear flag
		}EndIf();
	}

	if (SAVE_REPLAY_ANYWHERE_INDEX != -1) {
		If(OpenFlagReg, EQUAL_I, CODE_MENU_CLOSING); {
			SetRegister(Reg2, SAVE_REPLAY_ANYWHERE_INDEX);
			LWZ(Reg1, Reg2, Line::VALUE);
			If(Reg1, EQUAL_I, 1); {
				SetRegister(Reg1, 0);
				LBZ(Reg3, Reg2, Line::COLOR);
				STW(Reg1, Reg2, Line::VALUE); //clear setting
				ANDI(Reg3, Reg3, ~8);
				STB(Reg3, Reg2, Line::COLOR);

				IfInGame(3, false); {
					SaveReplay();
				}EndIf();
			}EndIf();
		}EndIf();
	}

	//stage collisions
	LoadByteToReg(Reg1, 0x80583FF8 + 1);
	If(Reg1, EQUAL_I, 2);
	{
		SetRegister(Reg2, 1);
		SetRegister(Reg3, SHOULD_RESET_STAGE_COLLISIONS_FLAG_LOC);
		STW(Reg2, Reg3, 0);
	} Else();
	{
		LoadWordToReg(Reg2, Reg3, SHOULD_RESET_STAGE_COLLISIONS_FLAG_LOC);
		If(Reg2, EQUAL_I, 1);
		{
			SetRegister(Reg2, 0);
			STW(Reg2, Reg3, 0);
			
			//reset
			SetRegister(3, 0x80672f40);
			SetRegister(4, 0);
			SetRegister(5, 1);
			CallBrawlFunc(0x8000d234); //setLayerDispStatus
			ADDI(Reg1, Reg1, 1);
		} EndIf();
	} EndIf();


	//hitbox display
	LoadByteToReg(Reg1, 0x80583FFC + 1);
	If(Reg1, EQUAL_I, 2);
	{
		SetRegister(Reg2, 1);
		SetRegister(Reg3, SHOULD_RESET_HITBOX_DISPLAY_FLAG_LOC);
		STW(Reg2, Reg3, 0);
	} Else();
	{
		LoadWordToReg(Reg2, Reg3, SHOULD_RESET_HITBOX_DISPLAY_FLAG_LOC);
		If(Reg2, EQUAL_I, 1);
		{
			SetRegister(Reg2, 0);
			STW(Reg2, Reg3, 0);

			//reset
			SetRegister(Reg1, 1);
			While(Reg1, LESS_OR_EQUAL_I, 5);
			{
				SetRegister(3, 0x80672f40);
				MR(4, Reg1);
				SetRegister(5, 1);
				CallBrawlFunc(0x8000d234); //setLayerDispStatus
				ADDI(Reg1, Reg1, 1);
			} EndWhile();
		} EndIf();
	} EndIf();
	

	//can't trust register values after here
	//need to change when save states are active again
	if (SAVE_STATES_INDEX != -1) {
		LoadWordToReg(Reg1, IS_IN_GAME_FLAG);
		If(Reg1, EQUAL_I, 1); {
			//LoadWordToReg(OpenFlagReg, CODE_MENU_CONTROL_FLAG);
			If(OpenFlagReg, EQUAL_I, CODE_MENU_CLOSED); {
				ANDI(Reg1, ButtonReg, BUTTON_DL);
				If(Reg1, NOT_EQUAL_I, 0); {
					SaveState();
				}Else(); {
					ANDI(Reg1, ButtonReg, BUTTON_DR);
					If(Reg1, NOT_EQUAL_I, 0); {
						RestoreState();
					}EndIf();
				}EndIf();
			}EndIf();
		}EndIf();
	}

	Label(NotLoaded);

	RestoreRegisters();
	ASMEnd(0x4e800020); //blr
}

void ApplyMenuSetting(int Index, int Destination, int reg1, int reg2, int size)
{
	if (Index != -1) {
		LoadWordToReg(reg1, Index + Line::VALUE);
		SetRegister(reg2, Destination);
		if (size == 4) { STW(reg1, reg2, 0); }
		if (size == 2) { STH(reg1, reg2, 0); }
		if (size == 1) { STB(reg1, reg2, 0); }
	}
}

void ExecuteAction(int ActionReg)
{
	int PageReg = 4;
	int LineReg = 5;
	int TempReg1 = 6;
	int TypeReg = 7;
	int TempReg2 = 8;
	int TempReg3 = 9;
	int TempReg4 = 10;
	int TempReg5 = 11;
	int TempReg6 = 12;

	int move = GetNextLabel();

	LoadWordToReg(PageReg, CURRENT_PAGE_PTR_LOC);
	LWZ(LineReg, PageReg, Page::CURRENT_LINE_OFFSET);
	ADD(LineReg, LineReg, PageReg);
	LBZ(TypeReg, LineReg, Line::TYPE);

	//move
	If(ActionReg, EQUAL_I, MOVE_UP); {
		//move up
		LHZ(TempReg1, LineReg, Line::UP);

		JumpToLabel(move);
	}EndIf();
	If(ActionReg, EQUAL_I, MOVE_DOWN); {
		//move down
		LHZ(TempReg1, LineReg, Line::DOWN);

		Label(move);
		Move(LineReg, PageReg, TempReg1, TempReg2, TempReg3);
	}EndIf();


	//change value
	If(ActionReg, EQUAL_I, INCREMENT); //increment
	IncreaseValue(LineReg, PageReg, TypeReg, TempReg1, TempReg2, TempReg3, TempReg4, TempReg5);
	EndIf(); //increment
	If(ActionReg, EQUAL_I, ENTER_SUB_MENU); //increment, if A is pressed
	IncreaseValue(LineReg, PageReg, TypeReg, TempReg1, TempReg2, TempReg3, TempReg4, TempReg5);
	EndIf(); //increment
	
	If(ActionReg, EQUAL_I, DECREMENT); //decrement
	DecreaseValue(LineReg, PageReg, TypeReg, TempReg1, TempReg2, TempReg3, TempReg4, TempReg5);
	EndIf(); //decrement


	//reset to defaults
	SetRegister(TempReg1, RESET_LINES_STACK_LOC);
	If(ActionReg, EQUAL_I, RESET_LINE); {
		ResetLine(LineReg, PageReg, TempReg1, TypeReg, TempReg2, TempReg3, TempReg4, 0);
	}EndIf();

	If(ActionReg, EQUAL_I, RESET_PAGE); {
		PushOnStack(PageReg, TempReg1, TempReg2);
	}EndIf();

	//reset page if applicable
	ResetPage(TempReg1, TempReg2, TempReg3, TempReg4, TempReg5, TempReg6, 3);

	//navigate menu
	If(ActionReg, EQUAL_I, ENTER_SUB_MENU); {
		EnterMenu(LineReg, PageReg, TypeReg, TempReg1, TempReg2);
	}EndIf();

	If(ActionReg, EQUAL_I, LEAVE_SUB_MENU); {
		LeaveMenu(PageReg, TempReg1, TempReg2, TempReg3, TempReg4, TempReg5, TempReg6, ActionReg);
	}EndIf();

	If(ActionReg, EQUAL_I, EXIT_MENU); {
		SetRegister(TempReg1, CODE_MENU_CLOSING);
		SetRegister(TempReg2, CODE_MENU_CONTROL_FLAG);
		STW(TempReg1, TempReg2, 0);
		ExitMenu();
	}EndIf();
}

void ResetLine(int LineReg, int PageReg, int StackReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3, bool isIndirectReset)
{
	int exitLabel = GetNextLabel();
	if (isIndirectReset)
	{
		LBZ(TempReg2, LineReg, Line::FLAGS);
		ANDI(TempReg2, TempReg2, Line::LINE_FLAGS_FIELDS::LINE_FLAG_IGNORE_INDIRECT_RESET);
		JumpToLabel(exitLabel, bCACB_NOT_EQUAL);
	}

	LBZ(TempReg2, LineReg, Line::COLOR);
	LWZ(TempReg1, PageReg, Page::NUM_CHANGED_LINES);
	RLWINM(TempReg3, TempReg2, 29, 31, 31);
	ANDI(TempReg2, TempReg2, ~0x8);
	SUBF(TempReg1, TempReg1, TempReg3);
	STB(TempReg2, LineReg, Line::COLOR);
	STW(TempReg1, PageReg, Page::NUM_CHANGED_LINES);

	If(TypeReg, LESS_OR_EQUAL_I, HAS_VALUE_LIMIT); {
		LWZ(TempReg1, LineReg, Line::DEFAULT);
		STW(TempReg1, LineReg, Line::VALUE);
	}Else(); If(TypeReg, EQUAL_I, SUB_MENU_LINE); {
		LHZ(TempReg1, LineReg, Line::SUB_MENU);
		ADD(TempReg1, TempReg1, PageReg);
		PushOnStack(TempReg1, StackReg, TempReg2);
	}EndIf(); EndIf();

	Label(exitLabel);
}

void ResetPage(int StackReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5, int TempReg6)
{
	IterateStack(TempReg1, StackReg, TempReg2); {
		SetRegister(TempReg6, 1);
		ADDI(TempReg2, TempReg1, Page::FIRST_LINE_OFFSET); //first line
		While(TempReg6, NOT_EQUAL_I, 0); {
			LBZ(TempReg3, TempReg2, Line::TYPE);
			ResetLine(TempReg2, TempReg1, StackReg, TempReg3, TempReg4, TempReg5, TempReg6, 1);

			LHZ(TempReg6, TempReg2, Line::SIZE);
			ADD(TempReg2, TempReg2, TempReg6); //next line
		}EndWhile();
	}IterateStackEnd();
}

void ExitMenu()
{
	LoadWordToReg(3, OLD_DEBUG_STATE_LOC);
	SetRegister(4, IS_DEBUG_PAUSED);
	STW(3, 4, 0);
}

void EnterMenu(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2)
{
	If(TypeReg, EQUAL_I, SUB_MENU_LINE); {
		LHZ(TempReg1, LineReg, SubMenu::SUB_MENU);
		ADD(TempReg2, PageReg, TempReg1);
		NEG(TempReg1, TempReg1);
		STW(TempReg1, TempReg2, Page::PREV_PAGE);
		SetRegister(TempReg1, CURRENT_PAGE_PTR_LOC);
		STW(TempReg2, TempReg1, 0);
	}EndIf();
}

void LeaveMenu(int PageReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5, int TempReg6, int ActionReg)
{
	LWZ(TempReg1, PageReg, Page::PREV_PAGE);
	If(TempReg1, EQUAL_I, 0); {
		/*LoadWordToReg(TempReg1, TempReg2, PREV_CODE_MENU_CONTROL_FLAG);
		STW(TempReg1, TempReg2, CODE_MENU_CONTROL_FLAG - PREV_CODE_MENU_CONTROL_FLAG);
		LoadWordToReg(TempReg1, OLD_DEBUG_STATE_LOC);
		SetRegister(TempReg2, IS_DEBUG_PAUSED);
		STW(TempReg1, TempReg2, 0);*/
		SetRegister(ActionReg, EXIT_MENU);
	}Else(); {
		ADD(TempReg2, PageReg, TempReg1);
		SetRegister(TempReg1, CURRENT_PAGE_PTR_LOC);
		STW(TempReg2, TempReg1, 0);

		//reg2 == super page
		LWZ(TempReg1, TempReg2, Page::CURRENT_LINE_OFFSET);
		ADD(TempReg1, TempReg1, TempReg2); //super page current line
		LBZ(TempReg3, TempReg1, Line::COLOR); //color of super line
		RLWINM(TempReg4, TempReg3, 29, 31, 31);
		LWZ(TempReg5, TempReg2, Page::NUM_CHANGED_LINES); //super page num changed lines
		ANDI(TempReg3, TempReg3, ~0x8);
		SUBF(TempReg5, TempReg5, TempReg4);
		LWZ(TempReg4, PageReg, Page::NUM_CHANGED_LINES);
		SetRegister(TempReg6, 0);
		ANDI(TempReg4, TempReg4, 0x1F);
		If(TempReg4, NOT_EQUAL_I, 0); {
			SetRegister(TempReg6, 1);
		}EndIf();
		ADD(TempReg5, TempReg5, TempReg6);
		RLWINM(TempReg6, TempReg6, 3, 0, 31); //<<3
		STW(TempReg5, TempReg2, Page::NUM_CHANGED_LINES);
		OR(TempReg3, TempReg3, TempReg6);
		STB(TempReg3, TempReg1, Line::COLOR);
	}EndIf();
}

void DecreaseValue(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5)
{
	If(TypeReg, LESS_OR_EQUAL_I, HAS_VALUE_LIMIT); {
		//has a value to change
		If(TypeReg, EQUAL_I, SELECTION_LINE); {
			//selection
			LWZ(TempReg1, LineReg, Line::VALUE);
			Decrement(TempReg1);

			If(TempReg1, LESS_I, 0); {
				LWZ(TempReg1, LineReg, Line::MAX);
			}EndIf();
			STW(TempReg1, LineReg, Line::VALUE);
		}Else(); If(TypeReg, EQUAL_I, INTEGER_LINE); {
			//integer
			LWZ(TempReg1, LineReg, Line::VALUE);
			LWZ(TempReg3, LineReg, Integer::SPEED);
			SUBF(TempReg1, TempReg1, TempReg3);

			LWZ(TempReg3, LineReg, Integer::MIN);
			// If Decremented Value is less than Min value...
			If(TempReg1, LESS, TempReg3); {
				// ... load flag byte...
				LBZ(TempReg2, LineReg, Line::FLAGS);
				// ... and check if the Wrap flag is enabled.
				ANDI(TempReg2, TempReg2, Integer::INT_FLAG_ALLOW_WRAP);
				BC(2, bCACB_EQUAL);
				// If it is, overwrite the MIN value in TempReg3 with the MAX value!
				LWZ(TempReg3, LineReg, Line::MAX);

				// And finally, copy the TempReg3 into TempReg1.
				// Final result being, if Wrap flag is enabled, TempReg1 == Max, if not, TempReg1 == Min.
				MR(TempReg1, TempReg3);
			}EndIf();

			STW(TempReg1, LineReg, Line::VALUE);
		}Else(); {
			//floating
			LFS(2, LineReg, Line::VALUE);
			LFS(1, LineReg, Floating::SPEED);
			FSUB(1, 2, 1);

			LFS(2, LineReg, Floating::MIN);
			FCMPU(1, 2, 0);
			BC(4, BRANCH_IF_FALSE, LT);
			LFS(2, LineReg, Floating::MAX);
			STFS(2, LineReg, Line::VALUE);
			B(2);
			STFS(1, LineReg, Line::VALUE);
		}EndIf(); EndIf(); //done

		//set changed flag
		LWZ(TempReg1, LineReg, Line::VALUE);
		LBZ(TempReg2, LineReg, Line::COLOR);
		LWZ(TempReg3, LineReg, Line::DEFAULT);
		LWZ(TempReg5, PageReg, Page::NUM_CHANGED_LINES);
		RLWINM(TempReg4, TempReg2, 29, 31, 31);
		ANDI(TempReg2, TempReg2, ~0x8);
		If(TempReg1, NOT_EQUAL, TempReg3); {
			//not default
			Increment(TempReg5);
			ORI(TempReg2, TempReg2, 0x8);
		}EndIf();
		SUBF(TempReg5, TempReg5, TempReg4);
		STB(TempReg2, LineReg, Line::COLOR);
		STW(TempReg5, PageReg, Page::NUM_CHANGED_LINES);
	}EndIf();
}

void IncreaseValue(int LineReg, int PageReg, int TypeReg, int TempReg1, int TempReg2, int TempReg3, int TempReg4, int TempReg5)
{
	If(TypeReg, LESS_OR_EQUAL_I, HAS_VALUE_LIMIT); {
		//has a value to change
		If(TypeReg, EQUAL_I, SELECTION_LINE); {
			//selection
			LWZ(TempReg1, LineReg, Line::VALUE);
			Increment(TempReg1);

			LWZ(TempReg2, LineReg, Line::MAX);
			If(TempReg1, GREATER, TempReg2); {
				SetRegister(TempReg1, 0);
			}EndIf();
			STW(TempReg1, LineReg, Line::VALUE);
		}Else(); If(TypeReg, EQUAL_I, INTEGER_LINE); {
			//integer
			LWZ(TempReg1, LineReg, Line::VALUE);
			LWZ(TempReg3, LineReg, Integer::SPEED);
			ADD(TempReg1, TempReg1, TempReg3);

			LWZ(TempReg3, LineReg, Integer::MAX);
			// If Incremented Value is greater than Max value...
			If(TempReg1, GREATER, TempReg3); {
				// ... load flag byte...
				LBZ(TempReg2, LineReg, Line::FLAGS);
				// ... and check if the Wrap flag is enabled.
				ANDI(TempReg2, TempReg2, Integer::INT_FLAG_ALLOW_WRAP);
				BC(2, bCACB_EQUAL);
				// If it is, overwrite the MAX value in TempReg3 with the MIN value!
				LWZ(TempReg3, LineReg, Line::MIN);

				// And finally, copy TempReg3 into TempReg1.
				// Final result being, if Wrap flag is enabled, TempReg1 == Min, if not, TempReg1 == Max.
				MR(TempReg1, TempReg3);
			}EndIf();

			STW(TempReg1, LineReg, Line::VALUE);
		}Else(); {
			//floating
			LFS(2, LineReg, Line::VALUE);
			LFS(1, LineReg, Floating::SPEED);
			FADD(1, 2, 1);

			LFS(2, LineReg, Floating::MAX);
			FCMPU(1, 2, 0);
			BC(4, BRANCH_IF_FALSE, GT);
			LFS(2, LineReg, Floating::MIN);
			STFS(2, LineReg, Line::VALUE);
			B(2);
			STFS(1, LineReg, Line::VALUE);
		}EndIf(); EndIf(); //done

		//set changed flag
		LWZ(TempReg1, LineReg, Line::VALUE);
		LBZ(TempReg2, LineReg, Line::COLOR);
		LWZ(TempReg3, LineReg, Line::DEFAULT);
		LWZ(TempReg5, PageReg, Page::NUM_CHANGED_LINES);
		RLWINM(TempReg4, TempReg2, 29, 31, 31);
		ANDI(TempReg2, TempReg2, ~0x8);
		If(TempReg1, NOT_EQUAL, TempReg3); {
			//not default
			Increment(TempReg5);
			ORI(TempReg2, TempReg2, 0x8);
		}EndIf();
		SUBF(TempReg5, TempReg5, TempReg4);
		STB(TempReg2, LineReg, Line::COLOR);
		STW(TempReg5, PageReg, Page::NUM_CHANGED_LINES);
	}EndIf();
}

void Move(int LineReg, int PageReg, int NextLineOffset, int TempReg1, int TempReg2) {
	LBZ(TempReg2, LineReg, Line::COLOR);
	XORI(TempReg2, TempReg2, 0x4);
	STB(TempReg2, LineReg, Line::COLOR);

	STW(NextLineOffset, PageReg, Page::CURRENT_LINE_OFFSET);

	ADD(TempReg1, NextLineOffset, PageReg);
	LBZ(TempReg2, TempReg1, Line::COLOR);
	XORI(TempReg2, TempReg2, 0x4);
	STB(TempReg2, TempReg1, Line::COLOR);
}

void GetActionFromInputs(int ButtonReg, int ControlStickXReg, int ControlStickYReg, int ResultReg)
{
	//action checks at the bottom have a higher priority
	SetRegister(ResultReg, NO_ACTION); //default

	//set Z for fast
	ANDI(4, ButtonReg, BUTTON_Z);
	If(4, NOT_EQUAL_I, 0); {
		//set frame timers to 3
		LoadWordToReg(4, 7, MOVE_FRAME_TIMER_LOC);
		LoadWordToReg(6, 8, INCREMENT_FRAME_TIMER_LOC);
		SetRegister(5, 3);
		If(4, GREATER, 5); {
			STW(5, 7, 0);
		}EndIf();
		If(6, GREATER, 5); {
			STW(5, 8, 0);
		}EndIf();
	}EndIf();

	//move
	ANDI(4, ButtonReg, BUTTON_DU);
	If(4, NOT_EQUAL_I, 0);
	SetRegister(ControlStickYReg, MOVE_THRESHHOLD);
	EndIf();

	ANDI(4, ButtonReg, BUTTON_DD);
	If(4, NOT_EQUAL_I, 0);
	SetRegister(ControlStickYReg, -MOVE_THRESHHOLD);
	EndIf();

	SetControlStickAction(ControlStickYReg, MOVE_FRAME_TIMER_LOC, MOVE_NUM_WAIT_FRAMES, FIRST_MOVE_NUM_WAIT_FRAMES, MOVE_THRESHHOLD, MOVE_UP, MOVE_DOWN, ResultReg);

	//increment
	ANDI(4, ButtonReg, BUTTON_DR);
	If(4, NOT_EQUAL_I, 0);
	SetRegister(ControlStickXReg, INCREMENT_THRESHHOLD);
	EndIf();

	ANDI(4, ButtonReg, BUTTON_DL);
	If(4, NOT_EQUAL_I, 0);
	SetRegister(ControlStickXReg, -INCREMENT_THRESHHOLD);
	EndIf();

	SetControlStickAction(ControlStickXReg, INCREMENT_FRAME_TIMER_LOC, INCREMENT_NUM_WAIT_FRAMES, FIRST_INCREMENT_NUM_WAIT_FRAMES, INCREMENT_THRESHHOLD, INCREMENT, DECREMENT, ResultReg);


	ANDI(4, ButtonReg, TRIGGER_RESET_LINE_BUTTON);
	If(4, NOT_EQUAL_I, 0); //enter sub menu
	SetRegister(ResultReg, RESET_LINE);
	EndIf(); //enter sub menu

	ANDI(4, ButtonReg, TRIGGER_RESET_PAGE_BUTTON);
	If(4, NOT_EQUAL_I, 0); //leave sub menu
	SetRegister(ResultReg, RESET_PAGE);
	EndIf(); //leave sub menu

	ANDI(4, ButtonReg, TRIGGER_ENTER_SUB_MENU_BUTTON);
	If(4, NOT_EQUAL_I, 0); //enter sub menu
	SetRegister(ResultReg, ENTER_SUB_MENU);
	EndIf(); //enter sub menu

	ANDI(4, ButtonReg, TRIGGER_LEAVE_SUB_MENU_BUTTON);
	If(4, NOT_EQUAL_I, 0); //leave sub menu
	SetRegister(ResultReg, LEAVE_SUB_MENU);
	EndIf(); //leave sub menu

	ANDI(4, ButtonReg, BUTTON_START);
	If(4, NOT_EQUAL_I, 0); //leave menu
	SetRegister(ResultReg, EXIT_MENU);
	EndIf(); //leave menu
}

void SetControlStickAction(int StickValReg, int TimerLoc, int NumWaitFrames, int FirstTimeNumWaitFrames, int Threshhold, int PositiveAction, int NegativeAction, int ResultReg)
{
	int FrameTimerReg = 6;
	LoadWordToReg(FrameTimerReg, 7, TimerLoc);
	ABS(8, StickValReg, 3);
	Decrement(FrameTimerReg);
	If(8, GREATER_OR_EQUAL_I, Threshhold); {
		If(FrameTimerReg, LESS_OR_EQUAL_I, 0); {
			If(FrameTimerReg, LESS_I, 0); {
				//first move
				SetRegister(FrameTimerReg, FirstTimeNumWaitFrames - NumWaitFrames);
			}EndIf();
			ADDI(FrameTimerReg, FrameTimerReg, NumWaitFrames + 1);
			SetRegister(ResultReg, NegativeAction);
			If(StickValReg, GREATER_OR_EQUAL_I, Threshhold); {
				SetRegister(ResultReg, PositiveAction);
			}EndIf();
		}EndIf();
	}Else(); {
		SetRegister(FrameTimerReg, 0);
	}EndIf();
	STW(FrameTimerReg, 7, 0);
}

void printFPS() {
	int reg1 = 31;
	int reg2 = 30;
	int reg3 = 29;
	int reg4 = 28;
	int reg5 = 27;
	int reg6 = 26;
	int reg7 = 25;
	int reg8 = 24;
	int fontHolderReg = 23;
	int fontHolderReg2 = 22;

	LoadWordToReg(reg1, FPS_DISPLAY_INDEX + Line::VALUE);
	If(reg1, EQUAL_I, 1);
	{
		SetRegister(reg1, 0);
		LWZ(fontHolderReg, 13, -0x42AC); //save font ptr if exist
		LWZ(fontHolderReg2, 13, -0x42A8); //save other font ptr if exist
		STW(reg1, 13, -0x42AC); //clear font ptr
		STW(reg1, 13, -0x42A8); //clear other font ptr

		SetRegister(reg1, STRING_BUFFER + 0x80);
		WriteStringToMem("FPS: %2d", reg1);
		LoadHalfToReg(reg2, 0x805a0856);
		SprintF(reg1, { reg2 });
		SetRegister(reg1, STRING_BUFFER - 1);
		MR(reg6, 3);
		SetRegister(reg3, DRAW_SETTINGS_BUFFER_LOC);

		SetupPrintText(reg3);

		SetRegs(3, { (int) 0x805B71F0, 0 });
		CallBrawlFunc(0x801f51dc);

		
		SetRegister(reg4, 0x805b6df8);
		LFS(1, reg4, 0); //scale factor

		SetFloatingRegister(2, reg4, 0.33);
		FMULS(2, 2, 1);
		STFS(2, reg3, 0x50); //size

		/*SetFloatingRegister(2, reg4, -160.0);
		FDIVS(2, 2, 1);
		STFS(2, reg3, DRAW_BUFFER_XPOS_OFFSET); //x pos

		SetFloatingRegister(2, reg4, -128.0);
		FDIVS(2, 2, 1);
		STFS(2, reg3, DRAW_BUFFER_YPOS_OFFSET); //y pos*/


		//SetRegister(reg4, GetHexFromFloat(0.10));
		//SetRegister(reg4, GetHexFromFloat(0.15));
		//STW(reg4, reg3, 0x50); //set size

		//set initial text pos
		SetRegister(reg4, GetHexFromFloat(-165));
		SetRegister(reg5, GetHexFromFloat(-115));
		//SetRegister(reg4, GetHexFromFloat(-192));
		//SetRegister(reg5, GetHexFromFloat(-128));
		SetTextPos(reg4, reg5, reg3);

		LoadHalfToReg(reg4, 0x805a0856);
		If(reg2, LESS_I, 60);
		{
			SetRegister(reg2, RED);
		} Else();
		{
			SetRegister(reg2, BLUE);
		} EndIf();
		SetTextColor(reg2, reg3);

		STW(fontHolderReg, 13, -0x42AC);
		STW(fontHolderReg2, 13, -0x42A8);
		PrintString(reg1, reg6, reg3);
	} EndIf();

}

void PrintCodeMenu()
{
	ASMStart(0x80017928, "[CM: Code Menu] Print Code Menu");
	vector<int> FPRegs(14);
	iota(FPRegs.begin(), FPRegs.end(), 0);
	SaveRegisters(FPRegs);

	int Reg1 = 31;
	int Reg2 = 30;
	int Reg3 = 29;
	int Reg4 = 28;
	int Reg5 = 27;
	int Reg6 = 26;
	int Reg7 = 25;
	int Reg8 = 24;

	LoadWordToReg(Reg1, CODE_MENU_CONTROL_FLAG);
	LoadWordToReg(Reg2, Reg4, OLD_CAMERA_POS_LOC);
	OR(Reg3, Reg1, Reg2);
	If(Reg3, GREATER_OR_EQUAL_I, CODE_MENU_OPEN); {
		//code menu is open, or old camera pos is not 0
		SetRegister(Reg3, 0x805b6d20);
		LoadWordToReg(4, 5, CODE_MENU_NEED_TO_SAVE_CAMERA_MATRIX_FLAG_LOC);
		If(4, EQUAL_I, 0);
		{
			SetRegister(6, 1);
			STW(6, 5, 0);
			SetRegister(Reg2, CODE_MENU_OLD_CAMERA_MATRIX_LOC);
			SetRegister(6, 4 * 12);
			Memmove(Reg2, Reg3, 6);
		} EndIf();
		WriteVectorToMem(DEFAULT_CAMERA_MATRIX, Reg3);
		DrawBlackBackground();
	}EndIf();

	If(Reg1, EQUAL_I, CODE_MENU_OPEN); {
		SetRegister(Reg2, 0x805b6df8);
		SetRegister(Reg1, 0x3eb2b8c2);
		STW(Reg1, Reg2, 0);

		LoadWordToReg(Reg1, CURRENT_PAGE_PTR_LOC);
		SetRegister(Reg2, DRAW_SETTINGS_BUFFER_LOC);
		PrintPage(Reg1, Reg2, Reg3, Reg4, Reg5, Reg6, Reg7, Reg8);
	}Else(); {
		If(Reg2, NOT_EQUAL_I, 0); {
			//need to reset camera and wait a frame
			SetRegister(Reg3, 0x805b6df8);
			SetRegister(Reg1, 0);
			STW(Reg2, Reg3, 0); //reset camera
			STW(Reg1, Reg4, 0); //clear saved camera

			SetRegister(3, 0x805b6d20);
			SetRegister(4, CODE_MENU_OLD_CAMERA_MATRIX_LOC);
			SetRegister(Reg2, 4 * 12);
			Memmove(3, 4, Reg2);
			SetRegister(3, 0);
			SetRegister(4, CODE_MENU_NEED_TO_SAVE_CAMERA_MATRIX_FLAG_LOC);
			STW(3, 4, 0);
		}Else(); {
			//printFPS();
			CallBrawlFunc(0x8002e844); //render
			if (FPS_DISPLAY_INDEX != -1) {
				printFPS();
			}
		}EndIf();
	}EndIf();

	RestoreRegisters();
	ASMEnd();
}

void PrimeCodeMenu()
{
	GeckoIf(0x8119969c, EQUAL, 0x9421fff0);
	//r3 + 0x40 has location
	ASMStart(0x8119969c, "[CM: Code Menu] Prime Code Menu");
	SaveRegisters();

	int Reg1 = 31;
	int Reg2 = 30;
	int Reg3 = 29;
	int Reg4 = 28;
	int Reg5 = 27;
	int Reg6 = 26;
	int Reg7 = 25;
	int Reg8 = 24;

	LWZ(Reg3, 3, 0x54);
	If(Reg3, EQUAL_I, 1); {
		LWZ(Reg3, 3, 0x40);
		If(Reg3, EQUAL_I, 8); {
			//on tourney
			LoadWordToReg(Reg1, Reg2, CODE_MENU_CONTROL_FLAG);
			If(Reg1, NOT_EQUAL_I, CODE_MENU_OPEN); {
				SetRegister(Reg2, CODE_MENU_CONTROL_FLAG);
				SetRegister(Reg1, CODE_MENU_PRIMED);
				STW(Reg1, Reg2, 0);
			}EndIf();
		}EndIf();
	} EndIf();
	

	SetRegister(Reg2, ON_GROUP_RECORDS_FLAG_LOC);
	SetRegister(Reg1, 0);
	If(Reg3, EQUAL_I, 0x4E); {
		//SetRegister(Reg1, 1);
	}EndIf();
	STW(Reg1, Reg2, 0);

	RestoreRegisters();
	ASMEnd(0x9421fff0); //stwu sp, sp, -0x10
	GeckoEndIf();
}

void SetTextColor(int ColorReg, int SettingsPtrReg)
{
	STW(ColorReg, SettingsPtrReg, 0x8);
	STW(ColorReg, SettingsPtrReg, 0xC);
	STW(ColorReg, SettingsPtrReg, 0x10);
	STW(ColorReg, SettingsPtrReg, 0x14);
}

void SetTextXPos(int XPosReg, int SettingPtrReg)
{
	STW(XPosReg, SettingPtrReg, DRAW_BUFFER_XPOS_OFFSET);
}

void SetTextYPos(int YPosReg, int SettingPtrReg)
{
	STW(YPosReg, SettingPtrReg, DRAW_BUFFER_YPOS_OFFSET);
}

void SetTextPos(int XPosReg, int YPosReg, int SettingPtrReg)
{
	SetTextXPos(XPosReg, SettingPtrReg);
	SetTextYPos(YPosReg, SettingPtrReg);
}

void SetTextSize(int FPSizeReg, int SettingsPtrReg)
{
	STFS(FPSizeReg, SettingsPtrReg, 0x50);
}

void NewLine(int SettingsPtrReg)
{
	SetRegister(3, GetHexFromFloat(INITIAL_XPOS));
	LFS(0, SettingsPtrReg, DRAW_BUFFER_YPOS_OFFSET);
	SetFloatingRegister(1, 4, LINE_HEIGHT);
	FADD(0, 0, 1);
	STW(3, SettingsPtrReg, DRAW_BUFFER_XPOS_OFFSET);
	STFS(0, SettingsPtrReg, DRAW_BUFFER_YPOS_OFFSET);
}

//r3 is SettingsPtr, r4 is CharReg
void PrintChar(int SettingsPtrReg, int CharReg)
{
	SetArgumentsFromRegs(3, { SettingsPtrReg, CharReg });
	CallBrawlFunc(0x8006fe50); //printChar
}

//StringPtr should be 1 less than start
//StringPtr gets moved to end of string, numCharsReg gets set to 0
void PrintString(int StringPtrReg, int NumCharsReg, int SettingsPtrReg)
{
	While(NumCharsReg, GREATER_I, 0); //print loop

	LBZU(4, StringPtrReg, 1);
	PrintChar(SettingsPtrReg, 4);

	Decrement(NumCharsReg);
	EndWhile(); //print loop
}

void SetupPrintText(int SettingPtrReg)
{
	ADDI(3, SettingPtrReg, 0);
	SetRegs(4, { 0, 9 });
	CallBrawlFunc(0x8006a964); //SetDefaultEnv

	CallBrawlFunc(0x80019FA4); //(getManager[cameraManager])

	CallBrawlFunc(0x80018DE4); //(setGX[camera])

	SetRegister(3, DRAW_SETTINGS_BUFFER_LOC + 0x48);
	SetRegister(4, 0x80497e44);
	STW(4, 3, 0); //set font
}

void DrawBlackBackground()
{
	CallBrawlFunc(0x80019FA4); //(getManager[cameraManager])

	CallBrawlFunc(0x80018DE4); //(setGX[camera])

	CallBrawlFunc(0x8001A5C0); //gfDrawSetVtxColorPrimEnvrment

	vector<float> positions;
	float size = 0x1000;
	positions.push_back(-size);
	positions.push_back(size);
	positions.push_back(size);
	positions.push_back(size);
	positions.push_back(size);
	positions.push_back(-size);
	positions.push_back(-size);
	positions.push_back(-size);
	DrawPrimitive(PRIMITIVE_QUAD, positions, BLACK, 1);
}

void getSelectedLineNum(int PageReg, int resultReg) {
	LWZ(resultReg, PageReg, Page::CURRENT_LINE_OFFSET);
	ADD(resultReg, PageReg, resultReg); //ptr to selected line
	LBZ(resultReg, resultReg, Line::LINE_NUM);
}

void getNewHold(int PageReg, int resultReg, int reg1, int reg2) {
	LWZ(resultReg, PageReg, Page::PRINT_LOW_HOLD);
	getSelectedLineNum(PageReg, reg1);
	If(reg1, GREATER, resultReg);
	{
		MR(resultReg, reg1);
	} Else();
	{
		ADDI(reg2, reg1, SCROLL_DISTANCE - 5);
		If(reg2, LESS, resultReg);
		{
			MR(resultReg, reg2);
		} EndIf();
	} EndIf();
}

void PrintPage(int PageReg, int SettingsPtrReg, int Reg1, int Reg2, int Reg3, int Reg4, int Reg5, int Reg6)
{
	SetupPrintText(SettingsPtrReg);

	SetRegister(Reg5, LINE_COLOR_TABLE.table_start());

	//set initial text pos
	SetRegister(Reg1, GetHexFromFloat(INITIAL_XPOS));
	SetRegister(Reg2, GetHexFromFloat(INITIAL_YPOS));
	getNewHold(PageReg, Reg4, Reg6, Reg3);
	STW(Reg4, PageReg, Page::PRINT_LOW_HOLD); //save

	If(Reg4, GREATER_I, SCROLL_DISTANCE);
	{
		ADDI(Reg3, Reg4, -SCROLL_DISTANCE);
		MULLI(Reg3, Reg3, -LINE_HEIGHT);
		ConvertIntToFloat(Reg3, Reg4, Reg6);
		SetRegister(Reg3, STRING_BUFFER);
		STW(Reg6, Reg3, 0);
		LFS(0, Reg3, 0);
		STW(Reg2, Reg3, 0);
		LFS(1, Reg3, 0);
		FADDS(0, 0, 1);
		STFS(0, Reg3, 0);
		LWZ(Reg2, Reg3, 0);
	} EndIf();

	SetTextPos(Reg1, Reg2, SettingsPtrReg);

	SetRegister(Reg1, GetHexFromFloat(0.06));
	STW(Reg1, SettingsPtrReg, 0x50); //set size

	ADDI(Reg2, PageReg, Page::FIRST_LINE_OFFSET); //first line
	SetRegister(3, 1);
	While(3, NOT_EQUAL_I, 0); {
		PrintCodeMenuLine(Reg2, SettingsPtrReg, Reg5, Reg3, Reg4);

		LHZ(3, Reg2, Line::SIZE);
		LHZUX(3, Reg2, 3);
	}EndWhile();
}

void PrintCodeMenuLine(int LinePtrReg, int SettingsPtrReg, int ColorArrayPtrReg, int TempReg1, int TempReg2)
{
	LBZ(TempReg2, LinePtrReg, Line::TYPE);

	LBZ(TempReg1, LinePtrReg, Line::COLOR);
	LWZX(TempReg1, ColorArrayPtrReg, TempReg1);
	SetTextColor(TempReg1, SettingsPtrReg);

	LHZ(4, LinePtrReg, Line::TEXT_OFFSET);
	ADD(4, 4, LinePtrReg);

	If(TempReg2, EQUAL_I, PRINT_LINE); {
		LWZ(TempReg1, LinePtrReg, Line::PRINT_LINE_NUM_ARGS);
		LHZ(TempReg2, LinePtrReg, Line::SIZE);
		MULLI(TempReg1, TempReg1, 4);
		SUBF(TempReg2, TempReg2, TempReg1);
		LSWX(5, TempReg2, LinePtrReg, TempReg1);

		SetRegister(3, STRING_BUFFER);
		WriteIntToFile(0x4cc63182); //clclr 6, 6
		CallBrawlFunc(0x803f89fc); //sprintf
	} Else(); If(TempReg2, EQUAL_I, FLOATING_LINE); {
		LFS(1, LinePtrReg, Line::VALUE);
		SprintF(4, {},  { 1 }, -1);
	} Else(); {
		LWZ(5, LinePtrReg, Line::VALUE); //get setting

		If(TempReg2, EQUAL_I, SELECTION_LINE); {
			LWZ(TempReg2, LinePtrReg, Selection::SELECTION_LINE_SOURCE_SELECTION_INDEX);
			ADDI(TempReg1, TempReg2, Selection::SELECTION_LINE_OFFSETS_START);
			RLWINM(5, 5, 2, 0, 31); //<< 2
			LHZX(5, TempReg1, 5); //get string offset
			ADD(5, 5, TempReg2); //get string offset
		}EndIf();

		SprintF(4, { 5 });
	} EndIf(); EndIf();

	MR(TempReg2, 3); //num chars
	SetRegister(TempReg1, STRING_BUFFER - 1);
	PrintString(TempReg1, TempReg2, SettingsPtrReg);

	NewLine(SettingsPtrReg);
}

//requires 2 endifs
void GetArrayValueFromIndex(int ArrayLoc, int IndexReg, int min, int max, int ResultReg)
{
	If(IndexReg, LESS_OR_EQUAL_I, max); {
		If(IndexReg, GREATER_OR_EQUAL_I, min); {
			RLWINM(4, IndexReg, 2, 0, 31); //<< 2
			SetRegister(ResultReg, ArrayLoc - (min * 4));
			LWZX(ResultReg, ResultReg, 4);
		}
	}
}

void SaveReplay()
{
	int NTEBufferReg = 31;
	int SectionBufferReg = 30;
	int HighTimeReg = 29;
	int LowTimeReg = 28;
	int CryptoBufferReg = 27;
	int PathPtrReg = 26;
	int reg1 = 25;
	int reg2 = 24;
	int reg3 = 23;
	int reg4 = 22;
	int reg5 = 21;
	int reg6 = 20;
	int reg7 = 19;

#if DOLPHIN_BUILD
	SetRegister(reg5, STRING_BUFFER);
	WriteStringToMem("nand:/collect.vff\0"s, reg5);
	SetRegs(3, { DOLPHIN_MOUNT_VF_LOC, 0, STRING_BUFFER });
	CallBrawlFunc(0x80020f90); //mountVF
	SetRegs(3, { 0, 607500 * 3});
	CallBrawlFunc(0x801e1a80); //OSSleepTicks
#endif

	//set buffer ptrs
	SetRegister(NTEBufferReg, REPLAY_NTE_DATA_BUFFER_LOC);
	SetRegister(SectionBufferReg, REPLAY_CREATE_SECTION_BUFFER_LOC);
	SetRegister(CryptoBufferReg, REPLAY_CRYPTO_BUFFER_LOC);

	//setup buffers
	//section buffer
	SetRegister(3, 0);
	STW(3, SectionBufferReg, 0);
	STW(3, SectionBufferReg, 4);
	vector<int> is = { 0x4e0341de, (int) 0xe6bbaa41, 0x6419b3ea, (int) 0xe8f53bd9 };
	//crypto buffer
	vector<int> stupid = is;
	WriteVectorToMem(stupid, CryptoBufferReg);
	SetRegister(3, 0x2A);
	SetRegister(4, 1);
	STW(3, CryptoBufferReg, 0x18);
	STW(4, CryptoBufferReg, 0x1C);

	CallBrawlFunc(0x801e1b34); //OSGetTime
	MR(HighTimeReg, 3);
	MR(LowTimeReg, 4);

	SetRegs(3, { REPLAY_NTE_DATA_BUFFER_LOC, 42 });
	CallBrawlFunc(0x80152b5c); //ctnteFileReplay

	GetHeapAddress(HEAP_ADDRESS_TABLE.CACHED_REPLAY_HEAP, 4);
	ADDI(4, 4, REPLAY_HEAP_REPLAY_BUFFER_BEGIN_OFF);
	CallBrawlFunc(0x80152c4c); //setData

	SetArgumentsFromRegs(3, { SectionBufferReg, NTEBufferReg, HighTimeReg, LowTimeReg });
	SetRegs(7, { 0x2A, 0, 0 });
	CallBrawlFunc(0x80153610); //createSection

	LWZ(3, SectionBufferReg, 0);
	STW(3, CryptoBufferReg, 0x10);
	LWZ(3, 3, 0x1C);
	ADDI(3, 3, 0x20);
	STW(3, CryptoBufferReg, 0x14);
	ADDI(3, CryptoBufferReg, -832);
#if DOLPHIN_BUILD == false
	CallBrawlFunc(0x8003d1cc); //run crypto
#endif
	
	SetRegister(PathPtrReg, STRING_BUFFER + 0xA0);
	//WriteStringToMem("/LegacyTE/rp/rp_%d%d.bin\0"s, PathPtrReg);
	//SprintF(PathPtrReg, { HighTimeReg, LowTimeReg });
	SetRegister(reg1, STRING_BUFFER);
	SetArgumentsFromRegs(3, { HighTimeReg, LowTimeReg, reg1 });
	CallBrawlFunc(0x801e1d80); //OSTicksToCalenderTime
	LWZ(reg7, reg1, 0); //second
	LWZ(reg2, reg1, 4); //minute
	LWZ(reg3, reg1, 8); //hour
	LWZ(reg4, reg1, 0xC); //day
	LWZ(reg5, reg1, 0x10); //month
	LWZ(reg6, reg1, 0x14); //year
	ADDI(reg5, reg5, 1);
	SetRegister(3, 100);
	MR(4, reg6);
	MOD(reg6, 4, 3);
#if DOLPHIN_BUILD
	WriteStringToMem("/rp/rp_%02d%02d%02d_%02d%02d_%02d.bin\0"s, PathPtrReg);
#else
	WriteStringToMem("/" + MAIN_FOLDER + "/rp/rp_%02d%02d%02d_%02d%02d_%02d.bin\0"s, PathPtrReg);
#endif
	SprintF(PathPtrReg, { reg6, reg5, reg4, reg3, reg2, reg7 });
	SetRegister(PathPtrReg, STRING_BUFFER);
#if DOLPHIN_BUILD
	LWZ(reg2, SectionBufferReg, 0);
	LWZ(reg1, reg2, 0x1C);
	ADDI(reg1, reg1, 0x20);
#else
	LWZ(reg1, CryptoBufferReg, 0x2C); //size
	LWZ(reg2, CryptoBufferReg, 0x28); //ptr
#endif
	SetRegister(reg3, 0x2000);
	Allocate(reg3);
	MR(reg3, 1);
	ADDI(1, 3, 0x1F00);
	MR(reg4, 3);

#if DOLPHIN_BUILD
	WriteFileToVF(PathPtrReg, reg1, reg2);

	SetRegs(3, { DOLPHIN_MOUNT_VF_LOC, 0 });
	CallBrawlFunc(0x80021038); //unmountVF
	SetRegister(reg1, 0);
	SetRegister(reg2, DOLPHIN_MOUNT_VF_LOC);
	STW(reg1, reg2, 0);
#else
	WriteFileToSD(PathPtrReg, reg1, reg2);
#endif
	MR(1, reg3);
	FreeMem(reg4);

	//deallocate
	MR(3, NTEBufferReg);
	SetRegister(4, 0);
	CallBrawlFunc(0x80152bdc); //dtnteFileReplay
	LWZ(reg1, CryptoBufferReg, 0x28);
	FreeMem(reg1);
	LWZ(reg1, SectionBufferReg, 0);
	FreeMem(reg1);
}

//requires 3 endifs
//r3 returns setting address
void RunIfPortToggle(int ARRAY_LOC, int PortReg) {
	GetArrayValueFromIndex(ARRAY_LOC, PortReg, 0, 3); {
		LWZ(5, 3, Line::VALUE); //get setting
		If(5, EQUAL_I, 1); {
		}
	}
}