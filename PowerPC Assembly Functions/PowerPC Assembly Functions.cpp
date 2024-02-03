#include "stdafx.h"
#include "PowerPC Assembly Functions.h"
#include <iostream>


#if BUILD_TYPE == PROJECT_PLUS // If the program is configured by default for P+ builds...
	#if PROJECT_PLUS_EX_BUILD // ... and this is a P+EX build... 
		unsigned long characterListVersion = characterListVersions::clv_PPEX_WALUIGI; // ... we'll default to the current version's character list,
		string MAIN_FOLDER = "P+EX/./."; // use the "P+EX" base directory,
		std::string MENU_NAME = "Project+Ex Code Menu"; // and use the P+EX menu name.

	#else // Otherwise...
		unsigned long characterListVersion = characterListVersions::clv_PPLUS; // ... we'll use the regular P+ Character List,
		string MAIN_FOLDER = "Project+"; // use the "Project+" directory,
		std::string MENU_NAME = "Project+ Code Menu"; // and use the P+ menu name.
	#endif
#else // If it's not for P+ builds...
	unsigned long characterListVersion = characterListVersions::clv_PROJECTM; // ... then we'll default to the Project M character list,
	string MAIN_FOLDER = "LegacyTE"; // use the LegacyTE directory (this can be changed, this is just what it was originally),
	std::string MENU_NAME = "Legacy TE 2.5 Code Menu"; // and use the LTE menu name.
#endif

const std::array<std::string, characterListVersions::__clv_Count> characterListVersionNames = 
{
	"vBrawl",
	"vBrawl+ (Sopo, Giga Bowser, WarioMan)",
	"Project M (Roy, Mewtwo)",
	"Project+ (Knuckles)",
	"P+EX (Ridley)",
	"P+EX (Waluigi)",
	"P+EX (Alloys)",
	"P+EX (Dark Samus)",
	"P+EX (Sceptile)",
};

bool CONFIG_OUTPUT_ASM_INSTRUCTION_DICTIONARY = 0;
bool CONFIG_DISABLE_ASM_DISASSEMBLY = 0;
bool CONFIG_ENABLE_ASM_HEX_COMMENTS = 0;
bool CONFIG_DELETE_ASM_TXT_FILE = 1;
bool CONFIG_ALLOW_IMPLICIT_OPTIMIZATIONS = 0;
bool CONFIG_ALLOW_BLA_FUNCTION_CALLS = 0;


bool setMAIN_FOLDER(std::string mainFolderIn)
{
	bool result = 0;

	if (mainFolderIn.size() == 0x8)
	{
		MAIN_FOLDER = mainFolderIn;
		result = 1;
	}

	return result;
}

bool CUSTOM_NAME_SUPPLIED = 0;

fstream WPtr;
std::vector<ledger::codeLedgerEntry> codeLedger = {};
std::size_t ledger::codeLedgerEntry::length()
{
	return (codeStartPos != SIZE_MAX && codeEndPos != SIZE_MAX && codeEndPos > codeStartPos) ? codeEndPos - codeStartPos : SIZE_MAX;
}
bool ledger::openLedgerEntry(std::string codeName, std::string codeBlurb)
{
	bool result = 0;

	if (codeLedger.empty() || codeLedger.back().codeEndPos != SIZE_MAX)
	{
		codeLedger.push_back(ledger::codeLedgerEntry(codeName, WPtr.tellp(), codeBlurb));
		result = 1;
	}
	else
	{
		std::cerr << "[ERROR] Failed to open code ledger entry (\"" << codeName << "\"):\n" <<
			"\tPrevious entry (\"" << codeLedger.back().codeName << "\") isn't closed!\n";
	}

	return result;
}
bool ledger::closeLedgerEntry()
{
	bool result = 0;

	if (!codeLedger.empty() && codeLedger.back().codeEndPos == SIZE_MAX)
	{
		codeLedger.back().codeEndPos = WPtr.tellp();
		result = 1;
	}
	else
	{
		std::cerr << "[ERROR] Failed to close code ledger entry (\"" << codeLedger.back().codeName << "\"):\n" <<
			"\tIt's already closed!\n";
	}

	return result;
}

bool ledger::writeCodeToASMStream(std::ostream& output, std::istream& codeStreamIn, std::size_t expectedLength, const std::string codeNameIn, const std::string codeBlurbIn, bool codeUnattested, bool disableDisassembly)
{
	bool result = 0;

	// Determine Hashtag Border Length
	std::size_t hashtagStrLength = (codeNameIn.empty()) ? 0 : std::max((int)codeNameIn.size(), 20);
	if (!codeBlurbIn.empty())
	{
		std::size_t cursorBak = 0;
		std::size_t cursor = 0;
		while (cursor < codeBlurbIn.size())
		{
			cursorBak = cursor;
			cursor = codeBlurbIn.find('\n', cursor);
			if (cursor != std::string::npos)
			{
				hashtagStrLength = std::max((cursor - cursorBak) + 2, hashtagStrLength);
				cursor += 1;
			}
			else
			{
				hashtagStrLength = std::max((codeBlurbIn.size() - cursorBak) + 2, hashtagStrLength);
				cursor = std::string::npos;
			}
		}
	}
	if (hashtagStrLength > 0)
	{
		// Write Code Name, Blurb, and Hashtags
		output << std::string(hashtagStrLength, '#') << "\n";
		output << codeNameIn << "\n";
		if (!codeBlurbIn.empty())
		{
			std::stringstream blurbStream(codeBlurbIn);
			std::string currentLine = "";
			while (std::getline(blurbStream, currentLine))
			{
				output << "# " << currentLine << "\n";
			}
		}
		output << std::string(hashtagStrLength, '#') << "\n";
	}

	// If we're intended to do the disassembly...
	if (!disableDisassembly)
	{
		// ... then pass off to the disassembler.
		result = lava::gecko::parseGeckoCode(output, codeStreamIn, expectedLength, 0, 0, CONFIG_ENABLE_ASM_HEX_COMMENTS) == expectedLength;
	}
	// Otherwise...
	else
	{
		// ... we'll just do it in-place.
		std::size_t consumedBytes = 0;
		// We're effectively just writing the same base string over and over again, just with different numbers.
		// So set up a base line to start...
		std::string currentLine = "* XXXXXXXX XXXXXXXX\n";
		// ... then for as long as we still have bytes to pull...
		while (codeStreamIn.good() && consumedBytes < expectedLength)
		{

			// ... write the next 16 bytes into their rightful positions in our string.
			lava::readNCharsFromStream(&currentLine[2], codeStreamIn, 0x8);
			lava::readNCharsFromStream(&currentLine[11], codeStreamIn, 0x8);
			// From there, just print it to the output stream...
			output << currentLine;
			// ... and increment the byte tally.
			consumedBytes += 0x10;
		}
	}


	return result;
}

branchConditionAndConditionBit::branchConditionAndConditionBit(int BranchConditionIn, int ConditionBitIn, unsigned char ConditionRegFieldIn)
{
	BranchCondition = BranchConditionIn;
	if (ConditionRegFieldIn == UCHAR_MAX)
	{
		if (ConditionBitIn != INT_MAX)
		{
			assert(ConditionBitIn < 32 && "Invalid ConditionBitIn: Value must be less than 32; Condition Register is only 32 bits!");
		}
		ConditionBit = ConditionBitIn;
	}
	else
	{
		assert(ConditionRegFieldIn < 8 && "Invalid ConditionRegFieldIn: Value must be less than 8; there are only 8 Condition Register fields!");
		assert(ConditionBitIn < 4 && "Invalid ConditionBitIn: If ConditionRegFieldIn is specified, ConditionBitIn must only indicate bit, not field!");
		ConditionBit = ConditionBitIn + (4 * ConditionRegFieldIn);
	}
}
// Returns a copy of this bCACB, with the ConditionRegField set to the specified value!
branchConditionAndConditionBit branchConditionAndConditionBit::inConditionRegField(unsigned char ConditionRegFieldIn) const
{
	return branchConditionAndConditionBit(BranchCondition, ConditionBit % 4, ConditionRegFieldIn);
}

std::vector<std::streampos> LabelPosVec{};
std::vector<labels::labelJump> LabelJumpVec{};


//converts char hex digit to decimal
int HexToDec(char x)
{
	x = tolower(x);
	if (x <= '9')
	{
		return x - '0';
	}
	return x - 'a' + 10;
}

void CodeStart(string FilePath)
{
	WPtr.open(FilePath, fstream::out);
	if(!WPtr.is_open())
	{
		cout << "ERROR, output file not found\n";
		exit(0);
	}
}

void CodeEnd()
{
	CompleteJumps();

	assert(IfIndex == 0);

	WPtr.close();
}

void MakeGCT(string TextFilePath, string OldGCTFilePath, string NewGCTFilePath)
{
	fstream TextFilePtr(TextFilePath, fstream::in | fstream::binary);
	fstream OldGCTFilePtr(OldGCTFilePath, fstream::in | fstream::binary);
	fstream NewGCTFilePtr(NewGCTFilePath, fstream::out | fstream::binary);

	if (TextFilePtr.is_open() == false)
	{
		cout << "could not find text file\n";
		return;
	}
	if (OldGCTFilePtr.is_open() == false)
	{
		cout << "could not find source gct file\n";
		return;
	}
	if (NewGCTFilePtr.is_open() == false)
	{
		cout << "could not make new gct file\n";
		return;
	}

	string str = "";
	char input;
	while ((input = TextFilePtr.get()) != EOF)
	{
		input = (HexToDec(input) << 4) + HexToDec(TextFilePtr.get());
		str.push_back(input);
	}

	NewGCTFilePtr << OldGCTFilePtr.rdbuf();
	NewGCTFilePtr.seekg(-8, ios::end);
	for (auto x : str)
	{
		NewGCTFilePtr << x;
	}
	char end[8] = { 0xF0,0,0,0,0,0,0,0 };
	NewGCTFilePtr.write(end, 8);


	TextFilePtr.close();
	OldGCTFilePtr.close();
	NewGCTFilePtr.close();
}

bool MakeASM(string TextFilePath, string OutputAsmPath, bool disableDisassembly)
{
	ifstream textFile(TextFilePath);
	if (!textFile.is_open())
	{
		cout << "Error: Unable to open txt file";
		return false;
	}

	ofstream neoASMFile(OutputAsmPath);
	if (!codeLedger.empty() && neoASMFile.is_open())
	{
		std::string tempName = "";
		unsigned long unknownCount = 0x00;
		unsigned long unnamedCount = 0x00;

		textFile.seekg(0);
		for (unsigned long i = 0; i < codeLedger.size(); i++)
		{
			ledger::codeLedgerEntry* currEntry = &codeLedger[i];
			if (textFile.tellg() < currEntry->codeStartPos)
			{
				tempName = "Unattested Code " + std::to_string(unknownCount);
				std::size_t unattestedLength = currEntry->codeStartPos - textFile.tellg();
				ledger::writeCodeToASMStream(neoASMFile, textFile, unattestedLength, tempName, "", 1);
				neoASMFile << "\n";
				unknownCount++;
			}

			std::size_t entryLength = currEntry->length();
			if (entryLength != SIZE_MAX)
			{
				tempName = currEntry->codeName;
				if (tempName.empty() && !ALLOW_BLANK_CODE_NAMES_IN_ASM)
				{
					tempName = "Unnamed Code " + std::to_string(unnamedCount);
					unnamedCount++;
				}
				textFile.seekg(currEntry->codeStartPos);
				ledger::writeCodeToASMStream(neoASMFile, textFile, entryLength, tempName, currEntry->codeBlurb, 0, disableDisassembly);
				neoASMFile << "\n";
			}
		}

		textFile.close();
		if (CONFIG_DELETE_ASM_TXT_FILE)
		{
			std::filesystem::remove(TextFilePath);
		}
	}
	else
	{
		std::cerr << "[ERROR] Couldn't create ASM File (\"" << OutputAsmPath << "\")!\n";
	}
}


int GetHexFromFloat(float Value)
{
	int *b = (int *)&Value;
	return *b;
}
float GetFloatFromHex(int Value)
{
	float *f = (float*)&Value;
	return *f;
}

int GetShiftNum(int endPos)
{
	return (WORD_SIZE - 1 - endPos);
}

int GetOpSegment(int val, int size, int pos)
{
	int mask = (1 << size) - 1;
	val &= mask;
	val <<= GetShiftNum(pos);
	return val;
}

//menu text has byte with how long the string is in front
void WriteMenuTextToFile(string Text)
{
	WriteTextToFile(((char) Text.size()) + Text);
}

void WriteTextToFile(string Text)
{
	string Output = "";
	for (int i = 0; i < Text.size(); i++) {
		sprintf(OpHexBuffer, "%02X", Text[i]);
		Output += OpHexBuffer[0];
		Output += OpHexBuffer[1];
	}
	/*for (int i = 0; i < Text.size() % 4; i++) {
		Output += "00";
	}*/
	const char* ptr = Output.c_str();
	WPtr << ptr;
}

//writes in hex
void WriteIntToFile(int val)
{
	sprintf(OpHexBuffer, "%08X", val);
	WPtr << OpHexBuffer;
}

//use preproccesor defined values for comparisions
//definitions with _I treat the third argument as an immediate
void If(int Val1, int Comparision, int Val2)
{
	if(IfIndex >= MAX_IFS)
	{
		cout << "ERROR, too many ifs\n";
		exit(0);
	}
	if (Comparision >= 40)
	{
		Comparision -= 40;
		CMPLI(Val1, Val2, 0);
	}
	else if (Comparision >= 30)
	{
		Comparision -= 30;
		CMPL(Val1, Val2, 0);
	}
	else if(Comparision >= 10)
	{
		Comparision -= 10;
		CMPI(Val1, Val2, 0);
	}
	else
	{
		CMP(Val1, Val2, 0);
	}
	IfStartPos[IfIndex] = WPtr.tellp();
	IfConditionArray[IfIndex] = Comparision;
	WriteIntToFile(0);
	IfIndex++;
}

void EndIf()
{
	IfIndex--;
	assert(IfIndex >= 0);
	int holdPos = WPtr.tellp();
	WPtr.seekp(IfStartPos[IfIndex]);
	if(IfConditionArray[IfIndex] == IS_ELSE)
	{
		B(CalcBranchOffset(IfStartPos[IfIndex], holdPos));
	}
	else
	{
		int BranchCond = BRANCH_IF_FALSE;
		if (IfConditionArray[IfIndex] > 2)
		{
			BranchCond = BRANCH_IF_TRUE;
			IfConditionArray[IfIndex] -= 3;
		}
		BC(CalcBranchOffset(IfStartPos[IfIndex], holdPos), BranchCond, IfConditionArray[IfIndex]);
	}
	
	WPtr.seekp(holdPos);
}

void Else()
{
	WriteIntToFile(0);
	EndIf();
	IfStartPos[IfIndex] = WPtr.tellp();
	IfStartPos[IfIndex] -= 8;
	IfConditionArray[IfIndex] = IS_ELSE;
	IfIndex++;
}

//use preproccesor defined values for comparisions
//definitions with _I treat the third argument as an immediate
void While(int Val1, int Comparision, int Val2)
{
	If(Val1, Comparision, Val2);

	if (Comparision >= 40)
	{
		OpHex = GetOpSegment(10, 6, 5); //cmpli
		OpHex |= GetOpSegment(Val2, 16, 31);
		Comparision -= 40;
		//CMPLI(Val1, Val2, 0);
	}
	else if (Comparision >= 30)
	{
		OpHex = GetOpSegment(31, 6, 5); //cmpl
		OpHex |= GetOpSegment(Val2, 5, 20);
		OpHex |= GetOpSegment(32, 10, 30);
		Comparision -= 30;
		//CMPL(Val1, Val2, 0);
	}
	else if(Comparision >= 10)
	{
		OpHex = GetOpSegment(11, 6, 5);//cmpi
		OpHex |= GetOpSegment(Val2, 16, 31);
		Comparision -= 10;
	}
	else
	{
		OpHex = GetOpSegment(31, 6, 5);//cmp
		OpHex |= GetOpSegment(Val2, 5, 20);
	}
	OpHex |= GetOpSegment(Val1, 5, 15);
	WhileCompareArray[WhileIndex] = OpHex;

	int BranchCond = BRANCH_IF_TRUE;
	if(Comparision > 2)
	{
		BranchCond = BRANCH_IF_FALSE;
		Comparision -= 3;
	}

	OpHex = GetOpSegment(16, 6, 5);
	OpHex |= GetOpSegment(BranchCond, 5, 10);
	OpHex |= GetOpSegment(Comparision, 5, 15);
	WhileConditionArray[WhileIndex] = OpHex;

	WhileIndex++;
}

void EndWhile()
{
	WhileIndex--;
	WriteIntToFile(WhileCompareArray[WhileIndex]);
	WhileConditionArray[WhileIndex] |= GetOpSegment(CalcBranchOffset(WPtr.tellp(), IfStartPos[IfIndex - 1] + 8), 14, 29);
	WriteIntToFile(WhileConditionArray[WhileIndex]);

	EndIf();
}

void SetRegister(int Register, int value)
{
	if ((value & 0xffff0000))
	{
		ADDIS(Register, 0, (value & 0xffff0000) >> 16);
		if (value & 0xffff)
		{
			ORI(Register, Register, (value & 0xffff));
		}
	}
	else
	{
		ADDI(Register, 0, (value & 0xffff));
		if (value & 0x8000)
		{
			ANDI(Register, Register, 0xFFFF);
		}
	}
}

void SetRegister(int Register, string value)
{
	int val = 0;
	for (int i = 0; i < value.size() && i < 4; i++)
	{
		val |= value[i] << ((3 - i) * 8);
	}
	SetRegister(Register, val);
}

void SetFloatingRegister(int FPReg, int TempReg1, int TempReg2, float Value)
{
	int *b = (int *)&Value;
	SetRegister(TempReg1, SET_FLOAT_REG_TEMP_MEM);

	SetRegister(TempReg2, *b);
	STW(TempReg2, TempReg1, 0);

	LFS(FPReg, TempReg1, 0);
}

void SetFloatingRegister(int FPReg, int TempReg, float Value)
{
	SetRegister(TempReg, GetHexFromFloat(Value));
	STW(TempReg, 1, -0x10);
	LFS(FPReg, 1, -0x10);
}

void LoadWordToReg(int Register, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Register, 0, (Address & 0xFFFF0000) >> 16);
		LWZ(Register, Register, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Register, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LWZ(Register, Register, (Address & 0xFFFF));
	}
}

void LoadHalfToReg(int Register, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Register, 0, (Address & 0xFFFF0000) >> 16);
		LHZ(Register, Register, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Register, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LHZ(Register, Register, (Address & 0xFFFF));
	}
}

void LoadByteToReg(int Register, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Register, 0, (Address & 0xFFFF0000) >> 16);
		LBZ(Register, Register, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Register, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LBZ(Register, Register, (Address & 0xFFFF));
	}
}

//Reg takes on the value of address
void LoadWordToReg(int DestReg, int Reg, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Reg, 0, (Address & 0xFFFF0000) >> 16);
		LWZU(DestReg, Reg, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Reg, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LWZU(DestReg, Reg, (Address & 0xFFFF));
	}
}

//Reg takes on the value of address
void LoadHalfToReg(int DestReg, int Reg, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Reg, 0, (Address & 0xFFFF0000) >> 16);
		LHZU(DestReg, Reg, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Reg, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LHZU(DestReg, Reg, (Address & 0xFFFF));
	}
}

//Reg takes on the value of address
void LoadByteToReg(int DestReg, int Reg, int Address)
{
	if ((Address & 0xFFFF) < 0x8000)
	{
		ADDIS(Reg, 0, (Address & 0xFFFF0000) >> 16);
		LBZU(DestReg, Reg, (Address & 0xFFFF));
	}
	else
	{
		ADDIS(Reg, 0, ((Address & 0xFFFF0000) >> 16) + 1);
		LBZU(DestReg, Reg, (Address & 0xFFFF));
	}
}

//takes an integer value from SourceReg and converts it into a 32 bit float that is stored in ResultReg
//the values of SourceReg and TempReg are overwritten
void ConvertIntToFloat(int SourceReg, int TempReg, int ResultReg)
{
	SetRegister(ResultReg, 0);

	If(SourceReg, NOT_EQUAL_I, 0); //is not 0

	If(SourceReg, LESS_I, 0); //is less than 0

	SetRegister(ResultReg, 0x80000000);
	NEG(SourceReg, SourceReg);

	EndIf(); //is less than 0 end

	//set mantissa
	CNTLZW(TempReg, SourceReg);
	ADDI(TempReg, TempReg, -8);
	RLWNM(SourceReg, SourceReg, TempReg, 9, 31);
	OR(ResultReg, ResultReg, SourceReg);

	//set exponent
	ADDI(TempReg, TempReg, -150); //LZ - ((32 - 8) + 126) = -exponent
	NEG(TempReg, TempReg);
	RLWINM(TempReg, TempReg, 23, 1, 0);
	OR(ResultReg, ResultReg, TempReg);

	EndIf(); //is not 0 end
}

//writes insert asm gecko code
//keeps track of how many lines are written
//writes extra 00000000 or 60000000 00000000 at end as necessary
//assumes ba = 0x80000000
void ASMStart(int BranchAddress, std::string name, std::string blurb)
{
	ledger::openLedgerEntry(name, blurb);

	int OpWord = (0xC2 << 24);
	if(BranchAddress >= 0x81000000)
	{
		OpWord += 0x01000000;
	}
	OpWord += BranchAddress & 0xFFFFFF;
	WriteIntToFile(OpWord);
	WriteIntToFile(0);
	ASMStartAddress = WPtr.tellp();
}

void ASMEnd()
{
	int HoldPos = WPtr.tellp();
	int numLines = HoldPos - ASMStartAddress;
	numLines /= 8;
	if(numLines % 2)
	{
		//odd number of ops
		WriteIntToFile(0);
		HoldPos += 8;
	}
	else
	{
		//even number of ops
		WriteIntToFile(0x60000000);
		WriteIntToFile(0);
		HoldPos += 16;
	}
	numLines /= 2;
	numLines++;
	WPtr.seekp(ASMStartAddress - 8);
	WriteIntToFile(numLines);
	WPtr.seekp(HoldPos);

	ledger::closeLedgerEntry();
}

void ASMEnd(int Replacement)
{
	WriteIntToFile(Replacement);
	ASMEnd();
}

void CodeRaw(std::string name, std::string blurb, const std::vector<unsigned long>& rawHexIn)
{
	ledger::openLedgerEntry(name, blurb);
	for (std::size_t i = 0; i < rawHexIn.size(); i++)
	{
		WriteIntToFile(rawHexIn[i]);
	}
	ledger::closeLedgerEntry();
}

void CodeRawStart(std::string name, std::string blurb)
{
	ledger::openLedgerEntry(name, blurb);
}

void CodeRawEnd()
{
	ledger::closeLedgerEntry();
}

void Label(int LabelNum)
{
	if (LabelNum >= LabelPosVec.size())
	{
		cout << "ERROR, invalid label num!\n";
		exit(0);
	}
	LabelPosVec[LabelNum] = WPtr.tellg();
}

int GetNextLabel()
{
	LabelPosVec.push_back(SIZE_MAX);
	return int(LabelPosVec.size() - 1);
}

void JumpToLabel(int LabelNum, branchConditionAndConditionBit conditionIn)
{
	LabelJumpVec.push_back(labels::labelJump(LabelNum, WPtr.tellp(), conditionIn));
	WriteIntToFile(0);
}
void JumpToLabel(int LabelNum, int BranchCondition, int ConditionBit)
{
	JumpToLabel(LabelNum, { BranchCondition, ConditionBit });
}

void CompleteJumps()
{
	int holdPos = WPtr.tellp();
	for(int i = 0; i < LabelJumpVec.size(); i++)
	{
		labels::labelJump* currJump = &LabelJumpVec[i];
		WPtr.seekp(currJump->jumpSourcePos);
		if (currJump->jumpCondition.BranchCondition != INT_MAX && currJump->jumpCondition.ConditionBit != INT_MAX)
		{
			BC(CalcBranchOffset(currJump->jumpSourcePos, LabelPosVec[currJump->labelNum]), currJump->jumpCondition.BranchCondition, currJump->jumpCondition.ConditionBit);
		}
		else
		{
			B(CalcBranchOffset(currJump->jumpSourcePos, LabelPosVec[currJump->labelNum]));
		}
	}
	WPtr.seekp(holdPos);
}

int CalcBranchOffset(int Location, int Target)
{
	return (Target - Location) / 8;
}

//copies null-terminated string of bytes from address in Source to address in Destination
//Destination and Source regs are set to the addresses of their respective null bytes, Temp is zeroed
void StrCpy(int Destination, int Source, int Temp)
{
	LBZ(Temp, Source, 0);
	STB(Temp, Destination, 0);
	While(Temp, NOT_EQUAL_I, 0); //not ended

	LBZU(Temp, Source, 1);
	STBU(Temp, Destination, 1);

	EndWhile(); //not ended
}

void GeckoStringWrite(string String, u32 Address)
{
	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(Address & 0xFE000000);
	}
	u32 Op = Address & 0x1FFFFFF;
	Op += 0x06000000;
	WriteIntToFile(Op);
	WriteIntToFile(String.size());
	int index = 0;
	u32 Word = 0;

	while (index < String.size())
	{
		if (String[index] == '|')
		{
			String[index] = 0;
		}
		Word += String[index] << (8 * (3 - (index % 4)));
		index++;
		if (index % 4 == 0)
		{
			WriteIntToFile(Word);
			Word = 0;
		}
	}

	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(0x80000000);
	}
}

void Gecko32BitWrite(int Address, int Value)
{
	u32 Op = Address & 0x1FFFFFF;
	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(Address & 0xFE000000);
	}
	Op += 0x04000000;
	WriteIntToFile(Op);
	WriteIntToFile(Value);
	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(0x80000000);
	}
}

void Gecko8BitWrite(int Address, int Value, int NumTimes)
{
	u32 Op = Address & 0x1FFFFFF;
	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(Address & 0xFE000000);
	}
	WriteIntToFile(Op);
	WriteIntToFile(Value & 0xFF + ((NumTimes - 1) << 16));
	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(0x80000000);
	}
}

void SetGeckoBaseAddress(int Address)
{
	WriteIntToFile(0x42000000);
	WriteIntToFile(Address);
}

void SetGeckoPointerAddress(int Address)
{
	WriteIntToFile(0x4A000000);
	WriteIntToFile(Address);
}

void LoadIntoGeckoPointer(int Address)
{
	WriteIntToFile(0x48000000);
	WriteIntToFile(Address);
}

//size is in bytes
//Writes for some reason!!!!???? Don't use!!!
void LoadIntoGeckoRegister(int Address, int Reg, int size) {
	WriteIntToFile(0x82000000 | ((size >> 1) << 20) | Reg);
	WriteIntToFile(Address);
}

//repeats is number of extra times after first to write
void StoreGeckoRegisterAt(int Address, int Reg, int size, int repeats) {
	WriteIntToFile(0x84000000 | ((size >> 1) << 20) | (repeats << 4) | Reg);
	WriteIntToFile(Address);
}

void GeckoIf(u32 Address, int Comparison, int Value)
{
	u32 Op = Address & 0x1FFFFFF;
	switch (Comparison) {
	case NOT_EQUAL:
		Op += 0x2000000;
		break;
	case GREATER:
		Op += 0x4000000;
		break;
	case LESS:
		Op += 0x6000000;
		break;
	}
	Op += 0x20000000;

	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(Address & 0xFE000000);
	}

	WriteIntToFile(Op);
	WriteIntToFile(Value);

	if (Address > 0x81000000)
	{
		SetGeckoBaseAddress(0x80000000);
	}
}

void GeckoEndIf() {
	WriteIntToFile(0xE0000000);
	WriteIntToFile(0x80008000);
}

void FindInArray(int ValueReg, int StartAddressReg, int numberOfElements, int elementOffset, int ResultReg, int TempReg)
{
	int EndOfSearch = GetNextLabel();

	SetRegister(ResultReg, 0);
	LBZ(TempReg, StartAddressReg, 0);
	While(ResultReg, LESS_I, numberOfElements); //search loop
	
	If(TempReg, EQUAL, ValueReg); //found target
	
	JumpToLabel(EndOfSearch);

	EndIf(); //found target

	LBZU(TempReg, StartAddressReg, elementOffset);
	ADDI(ResultReg, ResultReg, 1);

	EndWhile(); //search loop

	SetRegister(ResultReg, -1); //not found

	Label(EndOfSearch);
}

void FindInTerminatedArray(int ValueReg, int StartAddressReg, int endMarker, int elementOffset, int ResultReg, int TempReg, int searchSize)
{
	int EndOfSearch = GetNextLabel();

	SetRegister(ResultReg, 0);
	if (searchSize == 1) { LBZ(TempReg, StartAddressReg, 0); }
	else if (searchSize == 2) { LHZ(TempReg, StartAddressReg, 0); }
	else { LWZ(TempReg, StartAddressReg, 0); }
	While(TempReg, NOT_EQUAL_I, endMarker); //search loop
	
	If(TempReg, EQUAL, ValueReg); //found target
	
	JumpToLabel(EndOfSearch);

	EndIf(); //found target

	if (searchSize == 1) { LBZU(TempReg, StartAddressReg, elementOffset); }
	else if (searchSize == 2) { LHZU(TempReg, StartAddressReg, elementOffset); }
	else { LWZU(TempReg, StartAddressReg, elementOffset); }
	ADDI(ResultReg, ResultReg, 1);

	EndWhile(); //search loop

	SetRegister(ResultReg, -1); //not found

	Label(EndOfSearch);
}

void CallBrawlFunc(int Address, int addressReg) {
	// If BLAs are enabled, and the target address can be validly represented using a BLA...
	if (CONFIG_ALLOW_BLA_FUNCTION_CALLS && (Address >= 0x80000000) && (Address < 0x88000000) && !(Address & 0b11))
	{
		BLA(Address);
	}
	else
	{
		SetRegister(addressReg, Address);
		MTCTR(addressReg);
		BCTRL();
	}
}

//r3 returns ptr to memory
void Allocate(int SizeReg, int Heap)
{
	//SetRegister(3, 0x2A);
	SetRegister(3, Heap); //network
	//SetRegister(3, 18); //fighter1 resource
	CallBrawlFunc(GET_MEM_ALLOCATOR); //call getMemAllocator
	//set size
	ADDI(4, SizeReg, 0);
	CallBrawlFunc(ALLOC); //call allocator
}

//allocates a buffer of size SizeReg and stores its address at AddressReg if
//value at addressReg equals EmptyVal
void AllocateIfNotExist(int SizeReg, int AddressReg, int EmptyVal)
{
	LHZ(3, AddressReg, 0);
	If(3, EQUAL_I_L, EmptyVal);
	Allocate(SizeReg);
	STW(3, AddressReg, 0);
	EndIf();
}

//r3 = Dest, r4 = Source, r5 = size
void Memmove(int DestReg, int SourceReg, int SizeReg)
{
	if (DestReg != 3) { ADDI(3, DestReg, 0); }
	if (SourceReg != 4) { ADDI(4, SourceReg, 0); }
	if (SizeReg != 5) { ADDI(5, SizeReg, 0); }
	CallBrawlFunc(MEMMOVE); //Memmove
}

void SaveRegisters()
{
	SaveRegisters({});
}

void SaveRegisters(vector<int> FPRegs)
{
	FPPushRecords = FPRegs;
	int stackSize = 29 * 4 + FPRegs.size() * 8 + 8 + 8;
	STW(0, 1, -4);
	MFLR(0);
	STW(0, 1, 4);
	MFCTR(0);
	STW(0, 1, -8);

	int offset = -8;
	for (int x : FPRegs) {
		offset -= 8;
		STFD(x, 1, offset);
	}

	STWU(1, 1, -stackSize);
	STMW(3, 1, 8);
}

void SaveRegisters(int NumFPRegs)
{
	vector<int> FPRegs(NumFPRegs);
	iota(FPRegs.begin(), FPRegs.end(), 0);
	SaveRegisters(FPRegs);
}

void RestoreRegisters()
{
	int stackSize = 29 * 4 + FPPushRecords.size() * 8 + 8 + 8;

	LMW(3, 1, 8);
	ADDI(1, 1, stackSize);

	int offset = -8;
	for (int x : FPPushRecords) {
		offset -= 8;
		LFD(x, 1, offset);
	}
	
	LWZ(0, 1, -8);
	MTCTR(0);
	LWZ(0, 1, 4);
	MTLR(0);
	LWZ(0, 1, -4);
}

void SetRegs(int StartReg, vector<int> values)
{
	for (int x : values) {
		SetRegister(StartReg, x);
		StartReg++;
		if (StartReg > 31) {
			cout << "Too many values\n";
			exit(0);
		}
	}
}

void SetArgumentsFromRegs(int StartReg, vector<int> ValueRegs)
{
	for (int x : ValueRegs) {
		if (x != StartReg) {
			ADDI(StartReg, x, 0);
		}
		StartReg++;
		if (StartReg > 31) {
			cout << "Too many values\n";
			exit(0);
		}
	}
}

void SetFloatingArgumentsFromRegs(int FPStartReg, vector<int> FPValueRegs)
{
	for (int x : FPValueRegs) {
		if (x != FPStartReg) {
			FMR(FPStartReg, x);
		}
		FPStartReg++;
		if (FPStartReg > 31) {
			cout << "Too many values\n";
			exit(0);
		}
	}
}

void GXSetCullMode(int CullModeReg)
{
	if (CullModeReg != 3) { ADDI(3, CullModeReg, 0); }
	CallBrawlFunc(GX_SET_CULL_MODE);
}

void GXClearVtxDesc()
{
	CallBrawlFunc(GX_CLEAR_VTX_DESC);
}

void GXSetVtxDesc(int AttributeReg, int TypeReg)
{
	if (AttributeReg != 3) { ADDI(3, AttributeReg, 0); }
	if (TypeReg != 4) { ADDI(4, TypeReg, 0); }
	CallBrawlFunc(GX_SET_VTX_DESC);
}

void GXSetVtxAttrFmt(int vtxfmtReg, int attrReg, int countReg, int typeReg, int fracReg)
{
	SetArgumentsFromRegs(3, { vtxfmtReg, attrReg, countReg, typeReg, fracReg });
	CallBrawlFunc(GX_SET_VTX_ATTRIBUTE_FORMAT);
}

//copies the camera mtx pointed to by r3 into memory pointed to by r4
//r4 is unchanged
void GetCameraMtx(int StorageLocReg)
{
	ADDI(3, 13, -0x4170); //get camera obj
	if (StorageLocReg != 4) { ADDI(4, StorageLocReg, 0); }
	CallBrawlFunc(GET_CAMERA_MTX);
}

void GXSetCurrentMtx(int IDReg)
{
	if (IDReg != 3) { ADDI(3, IDReg, 0); }
	CallBrawlFunc(GX_SET_CURRENT_MTX);
}

void GXLoadPosMtxImm(int MtxPtrReg, int IDReg)
{
	SetArgumentsFromRegs(3, { MtxPtrReg, IDReg });
	CallBrawlFunc(GX_LOAD_POS_MTX_IMM);
}

void GXSetZMode(int CompareEnabledReg, int FuncTypeReg, int UpdateEnabledReg)
{
	SetArgumentsFromRegs(3, { CompareEnabledReg, FuncTypeReg, UpdateEnabledReg });
	CallBrawlFunc(GX_SET_Z_MODE);
}

void GXSetLineWidth(int WidthReg, int TexOffsetReg)
{
	SetArgumentsFromRegs(3, { WidthReg, TexOffsetReg });
	CallBrawlFunc(GX_SET_LINE_WIDTH);
}

void GXBegin(int TypeReg, int VtxAttrFmtReg, int NumVertsReg)
{
	SetArgumentsFromRegs(3, { TypeReg, VtxAttrFmtReg, NumVertsReg });
	CallBrawlFunc(GX_BEGIN);
}

void GFDrawSetupCoord2D()
{
	CallBrawlFunc(GF_DRAW_SETUP_COORD_2D);
}

void GXDrawSetVtxColorPrimEnviroment()
{
	CallBrawlFunc(GX_DRAW_SET_VTX_COLOR_PRIM_ENVIROMENT);
}

void FreeMem(int AddressReg)
{
	if (AddressReg != 3) { ADDI(3, AddressReg, 0); }
	CallBrawlFunc(GF_POOL_FREE);
}

void FreeMemFromFighter(int AddressReg)
{
	if (AddressReg != 3) { ADDI(3, AddressReg, 0); }
	CallBrawlFunc(0x8019d88c); //_dt
}

//EmptyVal is expected value of upper 16 bits if memory is not allocated
//stores value of EmptyVal into AddressReg
void FreeMemIfAllocd(int AddressReg, int EmptyVal)
{
	LHZ(3, AddressReg, 0);
	If(3, NOT_EQUAL_I_L, EmptyVal);
	LWZ(3, AddressReg, 0);
	FreeMem(3);
	SetRegister(3, EmptyVal);
	STW(3, AddressReg, 0);
	EndIf();
}

//calls free on a null teminated array of ptrs to allocd memory
void FreeAllocdArray(int AllocAddressReg)
{
	LWZU(3, AllocAddressReg, 4);
	While(3, NOT_EQUAL_I, 0);

	FreeMem(3);
	LWZU(3, AllocAddressReg, 4);

	EndWhile();
}

//location is where to save from, size is how much, saveTo is where to store the alloc ptr - 4
void SaveMem(int LocationReg, int SizeReg, int SaveToReg, int Heap)
{
	ADDI(5, SizeReg, 0x20);
	Allocate(5, Heap);
	STWU(3, SaveToReg, 4);
	STW(LocationReg, 3, 0);
	STW(SizeReg, 3, 4);
	ADDI(3, 3, 8);
	Memmove(3, LocationReg, SizeReg);
}

void SaveAllocedMem(int AddressReg, int SaveToReg, int reg1, int reg2, bool SaveHeader, int Heap)
{
	LWZ(reg1, AddressReg, -0x1C); //size
	if (SaveHeader) { ADDI(reg2, AddressReg, -0x20); }
	else { ADDI(reg1, reg1, -0x20); MR(reg2, AddressReg); }
	SaveMem(reg2, reg1, SaveToReg, Heap);
}

void Increment(int Reg)
{
	ADDI(Reg, Reg, 1);
}

void Decrement(int Reg)
{
	ADDI(Reg, Reg, -1);
}

void WriteStringToMem(string Text, int AddressReg)
{
	Text += "\0"s;
	for (int i = 0; i < Text.size(); i += 4) {
		SetRegister(4, Text.substr(i, min(4, (int) Text.size() - i)));
		STW(4, AddressReg, i);
	}
}

void WriteVectorToMem(vector<int> Values, int AddressReg)
{
	int offset = 0;
	for (int x : Values) {
		SetRegister(4, x);
		STW(4, AddressReg, offset);
		offset += 4;
	}
}

void WriteVectorToMem(vector<float> Values, int AddressReg)
{
	vector<int> V;
	for (float x : Values) {
		V.push_back(GetHexFromFloat(x));
	}
	WriteVectorToMem(V, AddressReg);
}

//start, end, and step have to be 16 bit signed integers
void CounterLoop(int CounterReg, int startVal, int endVal, int stepVal)
{
	if (startVal == endVal) {
		cout << "loop can't be entered" << endl;
		exit(-1);
	}
	SetRegister(CounterReg, startVal);
	if (stepVal < 0) {
		if (startVal < endVal) {
			cout << "Infinite loop" << endl;
			exit(-1);
		}

		While(CounterReg, GREATER_I, endVal);
	}
	else if (stepVal > 0) {
		if (startVal > endVal) {
			cout << "Infinite loop" << endl;
			exit(-1);
		}

		While(CounterReg, LESS_I, endVal);
	}
	else {
		cout << "step value can't be equal to 0" << endl;
		exit(-1);
	}


	CounterLoppRecords.push_back(stepVal << 16 | CounterReg);
}

void CounterLoopEnd()
{
	int CounterReg = CounterLoppRecords.back() & 0x1F;
	int stepVal = CounterLoppRecords.back() >> 16;

	ADDI(CounterReg, CounterReg, stepVal);
	EndWhile(); //button union loop

	CounterLoppRecords.pop_back();
}

//r3 returns num chars, max string length 0x100
void SprintF(int StrReg, vector<int> ValueRegs, int DestPtrReg)
{
	if (ValueRegs.size() > 6) {
		cout << "Too many arguments (sprintf)" << endl;
		exit(-1);
	}
	if (DestPtrReg == -1) {
		SetRegister(3, STRING_BUFFER);
	}
	else {
		MR(3, DestPtrReg);
	}
	SetArgumentsFromRegs(4, { StrReg });
	SetArgumentsFromRegs(5, ValueRegs);
	WriteIntToFile(0x4cc63182); //clclr 6, 6
	CallBrawlFunc(0x803f89fc); //sprintf
}

//r3 returns num chars, max string length 0x100
void SprintF(int StrReg, vector<int> ValueRegs, vector<int> FPValueRegs, int DestPtrReg)
{
	if (ValueRegs.size() > 6 || FPValueRegs.size() > 8) {
		cout << "Too many arguments (sprintf floating)" << endl;
		exit(-1);
	}
	if (DestPtrReg == -1) {
		SetRegister(3, STRING_BUFFER);
	}
	else {
		MR(3, DestPtrReg);
	}
	SetArgumentsFromRegs(4, { StrReg });
	SetArgumentsFromRegs(5, ValueRegs);
	SetFloatingArgumentsFromRegs(1, FPValueRegs);
	CMP(1, 1, 1);
	//WriteIntToFile(0x4cc63182); //clclr 6, 6
	CallBrawlFunc(0x803f89fc); //sprintf
}

void ClampStick(int FPXValReg, int FPYValReg)
{
	STFS(FPXValReg, 1, -0x30);
	STFS(FPYValReg, 1, -0x2C);
	ADDI(3, 1, -0x30);
	CallBrawlFunc(0x8004819c); //clamp stick
	LFS(FPXValReg, 1, -0x30);
	LFS(FPYValReg, 1, -0x2C);
}

void ConvertIntStickValsToFloating(int StickXReg, int StickYReg, int FPXResultReg, int FPYResultReg, int FPTempReg)
{
	SetRegister(4, 0x43300000);
	STW(4, 1, -0x30);
	LFD(FPTempReg, 2, -0x7B10);

	EXTSB(4, StickXReg);
	XORIS(4, 4, 0x8000);
	STW(4, 1, -0x2C);
	LFD(FPXResultReg, 1, -0x30);
	
	EXTSB(4, StickYReg);
	XORIS(4, 4, 0x8000);
	STW(4, 1, -0x2C);
	LFD(FPYResultReg, 1, -0x30);

	FSUBS(FPXResultReg, FPXResultReg, FPTempReg);
	FSUBS(FPYResultReg, FPYResultReg, FPTempReg);
}

void ConvertFloatingRegToInt(int FPReg, int ResultReg)
{
	FCTIWZ(FPReg, FPReg);
	STFD(FPReg, 1, -0x30);
	LWZ(ResultReg, 1, -0x2C);
}

void ConvertFloatingRegToInt(int FPReg, int ResultReg, int TempFPReg)
{
	if (TempFPReg == -1) {
		ConvertFloatingRegToInt(FPReg, ResultReg);
	}
	else {
		FCTIWZ(TempFPReg, FPReg);
		STFD(TempFPReg, 1, -0x30);
		LWZ(ResultReg, 1, -0x2C);
	}
}

void AddValueToByteArray(u32 value, vector<u8> &Array)
{
	for (int i = 0; i < 4; i++) {
		Array.push_back((value >> (3 * 8)) & 0xFF);
		value <<= 8;
	}
}

void AddValueToByteArray(u16 value, vector<u8> &Array)
{
	for (int i = 0; i < 2; i++) {
		Array.push_back((value >> 8) & 0xFF);
		value <<= 8;
	}
}

void AddValueToByteArray(u8 value, vector<u8> &Array)
{
	Array.push_back(value);
}

void AddValueToByteArray(int value, vector<u8> &Array)
{
	for (int i = 0; i < 4; i++) {
		Array.push_back((value >> (3 * 8)) & 0xFF);
		value <<= 8;
	}
}

void AddValueToByteArray(short value, vector<u8> &Array)
{
	for (int i = 0; i < 2; i++) {
		Array.push_back((value >> 8) & 0xFF);
		value <<= 8;
	}
}

void AddValueToByteArray(char value, vector<u8> &Array)
{
	Array.push_back(value);
}

void DrawPrimitive(int type, vector<float> Positions, int Color, int VTXAttrFrmt)
{
	vector<int> Colors(Positions.size() / 2, Color);
	DrawPrimitive(type, Positions, Colors, VTXAttrFrmt);
}

void DrawPrimitive(int type, vector<float> Positions, vector<int> Colors, int VTXAttrFrmt)
{
	if (Positions.size() != Colors.size() * 2) {
		cout << "Number of Positions and Colors don't match";
		exit(-1);
	}

	SetRegs(3, { type, VTXAttrFrmt, (int) Colors.size() });
	GXBegin(3, 4, 5);

	SetRegister(3, 0xCC010000);
	SetRegister(6, GetHexFromFloat(0.5));
	float lastX = Positions[0] - 1, lastY = Positions[1] - 1, lastC = Colors[0] - 1;
	for (int p = 0, c = 0; c < Colors.size(); p += 2, c++) {
		if (Positions[p] != lastX) { SetRegister(4, GetHexFromFloat(Positions[p])); lastX = Positions[p]; }
		if (Positions[p + 1] != lastY) { SetRegister(5, GetHexFromFloat(Positions[p + 1])); lastY = Positions[p+1]; }
		if (Colors[c] != lastC) { SetRegister(7, Colors[c]); lastC = Colors[c]; }
		STW(4, 3, -0x8000);
		STW(5, 3, -0x8000);
		STW(6, 3, -0x8000);
		STW(7, 3, -0x8000);
	}
}

void LoadVal(int AddressReg, int size, int offset, int ResultReg)
{
	if (size == 1) { LBZ(ResultReg, AddressReg, offset); }
	else if (size == 2) { LHZ(ResultReg, AddressReg, offset); }
	else { LWZ(ResultReg, AddressReg, offset); }
}

void GetValueFromPtrPath(int StartingAddress, vector<int> Path, int ResultReg)
{
	LoadWordToReg(ResultReg, StartingAddress);
	GetValueFromPtrPath(Path, ResultReg, ResultReg);
}

void GetValueFromPtrPath(vector<int> Path, int StartingReg, int ResultReg)
{
	LWZ(ResultReg, StartingReg, Path[0]);
	int i = 1;
	for (; i < Path.size() - 2; i++) {
		LWZ(ResultReg, ResultReg, Path[i]);
	}
	LoadVal(ResultReg, Path.back(), Path[i], ResultReg);
}

void AllocateStack(int size, int Address, int TempReg)
{
	size += 0x10;
	SetRegister(TempReg, size);
	Allocate(TempReg);
	SetRegister(TempReg, Address);
	STW(3, TempReg, 0);
	STW(3, 3, 0); //set ptr to begining
}

void PushOnStack(int ValueReg, int StackReg, int TempReg)
{
	LWZ(TempReg, StackReg, 0);
	STWU(ValueReg, TempReg, 4);
	STW(TempReg, StackReg, 0);
}

//requires endif
void PopOffStack(int ResultReg, int StatckReg, int TempReg)
{
	LWZ(TempReg, StatckReg, 0);
	LWZ(ResultReg, TempReg, 0);
	If(TempReg, NOT_EQUAL, StatckReg); {
		ADDI(TempReg, TempReg, -4);
		STW(TempReg, StatckReg, 0);
	}
}

void IterateStack(int ResultReg, int StackReg, int TempReg)
{
	StackIteratorRecords.push_back(GetNextLabel());
	Label(StackIteratorRecords.back());

	PopOffStack(ResultReg, StackReg, TempReg);
}

void IterateStackEnd()
{
	JumpToLabel(StackIteratorRecords.back());
	EndIf();
	StackIteratorRecords.pop_back();
}

void AllocateVector(int size, int Address, int reg)
{
	size += 0x10;
	SetRegister(reg, size);
	Allocate(reg);
	SetRegister(reg, Address);
	STW(3, reg, 0);
	STW(3, 3, 0); //set ptr to begining
}

void ClearVector(int VectorReg)
{
	STW(VectorReg, VectorReg, 0); //clear ptr to end
}

//make sure there's enough room in destination, and no overlap
void CopyVector(int SourceVector, int DestVector)
{
	LWZ(5, SourceVector, 0);
	SUBF(5, 5, SourceVector);
	ADDI(5, 5, 4);
	Memmove(DestVector, SourceVector, 5);
}

//don't change temp regs in loop
//requires endwhile
void IterateVector(int VectorReg, int IteratorReg, int TempReg1, int TempReg2, int StartReg)
{
	LWZ(TempReg1, VectorReg, 0); //ptr to end
	if (StartReg == -1) { MR(TempReg2, VectorReg); }
	else { ADDI(TempReg2, StartReg, -4); }

	While(TempReg2, LESS_L, TempReg1);
	LWZU(IteratorReg, TempReg2, 4);
}

//don't change temp regs in loop
//requires endwhile
void ReverseIterateVector(int VectorReg, int IteratorReg, int TempReg1, int TempReg2, int StartReg)
{
	//ptr to end
	if (StartReg != -1) { MR(TempReg1, StartReg); }
	else { LWZ(TempReg1, VectorReg, 0); }
	ADDI(TempReg2, VectorReg, 4);
	ADDI(TempReg1, TempReg1, 4);

	While(TempReg1, GREATER_L, TempReg2);
	LWZU(IteratorReg, TempReg1, -4);
}

void RemoveFromVector(int VectorReg, int ValueReg)
{
	FindInVector(VectorReg, ValueReg, 5); {
		ShiftVectorDown(VectorReg, 5);
	}EndIf();
}

//requires endif
void FindInVector(int VectorReg, int ValueReg, int ResultReg)
{
	int Found = GetNextLabel();
	int IteratorReg = 3;
	int EndPtrReg = 4;
	IterateVector(VectorReg, IteratorReg, EndPtrReg, ResultReg); {
		If(IteratorReg, EQUAL, ValueReg); {
			JumpToLabel(Found);
		}EndIf();
	}EndWhile();

	If(EndPtrReg, NOT_EQUAL, EndPtrReg); //always false
	Label(Found);
}

void ShiftVectorDown(int VectorReg, int StartReg)
{
	int IteratorReg = 6;
	int PtrReg = 8;
	ADDI(StartReg, StartReg, 4);
	IterateVector(VectorReg, IteratorReg, 7, PtrReg, StartReg); {
		STW(IteratorReg, PtrReg, -4);
	}EndWhile();

	PopOffStack(4, VectorReg); EndIf();
}

void Randi(int ResultReg, int MaxReg, int TempReg) {
	LWZ(TempReg, 13, -17256 + 4); //random value location
	MOD(ResultReg, TempReg, MaxReg);
}

/*void RandomCapped(int HighReg, int reg1, int ResultReg)
{
	LoadWordToReg(reg1, 0x805a0420 + 4);
	MOD(ResultReg, HighReg, reg1);
}*/

void WriteFileToSD(int PathReg, int SizeReg, int DataPtrReg)
{
	SetRegister(4, 0);
	SetRegister(3, WRITE_SD_FILE_HEADER_LOC);
	STW(PathReg, 3, 0);
	STW(4, 3, 4);
	STW(SizeReg, 3, 8);
	STW(DataPtrReg, 3, 0xC);
	STW(4, 3, 0x10);
	STW(4, 3, 0x14);
	CallBrawlFunc(0x8001d740); //writeSD
}

void WriteFileToVF(int PathReg, int SizeReg, int DataPtrReg)
{
	SetRegister(4, 0);
	SetRegister(3, WRITE_SD_FILE_HEADER_LOC);
	STW(PathReg, 3, 0);
	STW(4, 3, 4);
	STW(SizeReg, 3, 8);
	STW(DataPtrReg, 3, 0xC);
	STW(4, 3, 0x10);
	STW(4, 3, 0x14);
	CallBrawlFunc(0x8001d5a8); //writeVFFile
}

//reguires endif
void IfInGame(int reg, bool condition)
{
	LoadWordToReg(reg, IS_IN_GAME_FLAG);
	If(reg, EQUAL_I, condition);
}

//r3 returns new value
//r4 returns address
void ClearBitsFromMemory(short BitsToClear, int Address) {
	LoadWordToReg(3, 4, Address);
	ANDI(3, 3, ~BitsToClear);
	STW(3, 4, 0);
}

//updates AddressReg with offset
//r3 returns new value
void ClearBitsFromMemory(short BitsToClear, int AddressReg, int Offset) {
	LWZU(3, AddressReg, Offset);
	ANDI(3, 3, ~BitsToClear);
	STW(3, AddressReg, 0);
}

void GetSceneNum(int ResultReg) {
	LoadWordToReg(ResultReg, 0x805b4fd8 + 0xd4);
	LWZ(ResultReg, ResultReg, 0x10);
	If(ResultReg, NOT_EQUAL_I, 0); {
		LWZ(ResultReg, ResultReg, 8);
	} Else(); {
		SetRegister(ResultReg, -1);
	} EndIf();
}

void IfInVersus(int reg) {
	GetSceneNum(reg);
	If(reg, EQUAL_I, 0xA);
}

void LoadFile(string filePath, int destination, int reg1, int reg2, bool loadFromSD)
{
	SetRegister(reg1, STRING_BUFFER);

	SetRegister(reg2, STRING_BUFFER + 0x18);
	STW(reg2, reg1, 0); //file path ptr

	SetRegister(reg2, 0);
	STW(reg2, reg1, 4);
	STW(reg2, reg1, 8);
	STW(reg2, reg1, 0x10);

	SetRegister(reg2, destination);
	STW(reg2, reg1, 0xC); //storage loc

	SetRegister(reg2, -1);
	STW(reg2, reg1, 0x14);

	ADDI(reg2, reg1, 0x18);
	WriteStringToMem(filePath, reg2);

	MR(3, reg1);
	if (loadFromSD) {
		CallBrawlFunc(0x8001cbf4); //readSDFile
	}
	else {
		CallBrawlFunc(0x8001BF0C); //readFile
	}
}

void constrainFloat(int floatReg, int tempFReg, int tempReg, float min, float max) {
	SetFloatingRegister(tempFReg, tempReg, min);
	FCMPU(floatReg, tempFReg, 0);
	BC(2, BRANCH_IF_FALSE, LT);
	FMR(floatReg, tempFReg);

	SetFloatingRegister(tempFReg, tempReg, max);
	FCMPU(floatReg, tempFReg, 0);
	BC(2, BRANCH_IF_FALSE, GT);
	FMR(floatReg, tempFReg);
}

void constrainFloatDynamic(int floatReg, int minFReg, int maxFReg) {
	FCMPU(floatReg, minFReg, 0);
	BC(2, BRANCH_IF_FALSE, LT);
	FMR(floatReg, minFReg);

	FCMPU(floatReg, maxFReg, 0);
	BC(2, BRANCH_IF_FALSE, GT);
	FMR(floatReg, maxFReg);
}

void modifyInstruction(int instructionReg, int addressReg) {
	STW(instructionReg, addressReg, 0);
	//DCBF(0, addressReg);
	DCBST(0, addressReg);
	SYNC();
	ICBI(0, addressReg);
	ISYNC();
}

void IfInSSE(int reg1, int reg2) {
	LoadWordToReg(reg1, 0x805B50AC);
	LWZ(reg1, reg1, 0x10);
	LWZ(reg1, reg1, 0);
	SetRegister(reg2, 0x80702B60);
	If(reg1, EQUAL, reg2);
}

void IfNotInSSE(int reg1, int reg2) {
	LoadWordToReg(reg1, 0x805B50AC);
	LWZ(reg1, reg1, 0x10);
	LWZ(reg1, reg1, 0);
	SetRegister(reg2, 0x80702B60);
	If(reg1, NOT_EQUAL, reg2);
}

void GetHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int destinationReg)
{
	ADDIS(destinationReg, 0, HEAP_ADDRESS_TABLE.table_start() >> 0x10);
	LWZ(destinationReg, destinationReg, HEAP_ADDRESS_TABLE.header_relative_address_offset(heapIndex));
}

void LoadWordFromHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int loadDestinationReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	LWZ(loadDestinationReg, addressDestinationReg, offset);
}

void StoreWordToHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int sourceReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	STW(sourceReg, addressDestinationReg, offset);
}

void LoadHalfFromHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int loadDestinationReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	LHZ(loadDestinationReg, addressDestinationReg, offset);
}

void StoreHalfToHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int sourceReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	STH(sourceReg, addressDestinationReg, offset);
}

void LoadByteFromHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int loadDestinationReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	LBZ(loadDestinationReg, addressDestinationReg, offset);
}

void StoreByteToHeapAddress(_heapCacheTable::CachedHeaps heapIndex, int sourceReg, int addressDestinationReg, int offset)
{
	GetHeapAddress(heapIndex, addressDestinationReg);
	STB(sourceReg, addressDestinationReg, offset);
}


void ABS(int DestReg, int SourceReg, int tempReg)
{
	SRAWI(tempReg, SourceReg, 31);
	ADD(DestReg, SourceReg, tempReg);
	XOR(DestReg, DestReg, tempReg);
}

void ADD(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(266, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void ADDI(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(14, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void ADDIC(int DestReg, int SourceReg, int Immediate, bool SetConditionReg)
{
	// Op Code is 13 if we're updating the condition register, 12 if we aren't! 
	OpHex = GetOpSegment((SetConditionReg) ? 13 : 12, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void ADDIS(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(15, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void ADDME(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 15);
	OpHex |= GetOpSegment(234, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void AND(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(28, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

//SourceReg2 is complimented
void ANDC(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(60, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void ANDI(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(28, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}
	
void ANDIS(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(29, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void B(int JumpDist)
{
	OpHex = GetOpSegment(18, 6, 5);
	OpHex |= GetOpSegment(JumpDist, 24, 29);
	WriteIntToFile(OpHex);
}

void BA(int Address)
{
	Address /= 4;
	OpHex = GetOpSegment(18, 6, 5);
	OpHex |= GetOpSegment(Address, 24, 29);
	OpHex |= 2;
	WriteIntToFile(OpHex);
}

void BC(int JumpDist, branchConditionAndConditionBit conditionIn)
{
	BC(JumpDist, conditionIn.BranchCondition, conditionIn.ConditionBit);
}
//distance/4, branch if true/false, bit to check
void BC(int JumpDist, int BranchCondition, int ConditionBit)
{
	// If we're jumping backwards...
	if (JumpDist < 0)
	{
		// ... we need to invert the y-bit in BO.
		BranchCondition ^= 0b1;
	}

	OpHex = GetOpSegment(16, 6, 5);
	OpHex |= GetOpSegment(BranchCondition, 5, 10);
	OpHex |= GetOpSegment(ConditionBit, 5, 15);
	OpHex |= GetOpSegment(JumpDist, 14, 29);
	WriteIntToFile(OpHex);
}

void BCTR()
{
	WriteIntToFile(0x4e800420);
}

void BCTRL()
{
	WriteIntToFile(0x4e800421);
}

void BL(int JumpDist)
{
	OpHex = GetOpSegment(18, 6, 5);
	OpHex |= GetOpSegment(JumpDist, 24, 29);
	OpHex |= 1;
	WriteIntToFile(OpHex);
}

void BLA(int Address)
{
	Address /= 4;
	OpHex = GetOpSegment(18, 6, 5);
	OpHex |= GetOpSegment(Address, 24, 29);
	OpHex |= 3;
	WriteIntToFile(OpHex);
}

void BLR()
{
	WriteIntToFile(0x4e800020);
}

void CMP(int Reg1, int Reg2, int CondField)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(CondField, 3, 8);
	OpHex |= GetOpSegment(Reg1, 5, 15);
	OpHex |= GetOpSegment(Reg2, 5, 20);
	WriteIntToFile(OpHex);
}

void CMPI(int Reg, int Immediate, int CondField)
{
	OpHex = GetOpSegment(11, 6, 5);
	OpHex |= GetOpSegment(CondField, 3, 8);
	OpHex |= GetOpSegment(Reg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void CMPL(int Reg1, int Reg2, int CondField)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(CondField, 3, 8);
	OpHex |= GetOpSegment(Reg1, 5, 15);
	OpHex |= GetOpSegment(Reg2, 5, 20);
	OpHex |= GetOpSegment(32, 10, 30);
	WriteIntToFile(OpHex);
}

void CMPLI(int Reg, int Immediate, int CondField)
{
	OpHex = GetOpSegment(10, 6, 5);
	OpHex |= GetOpSegment(CondField, 3, 8);
	OpHex |= GetOpSegment(Reg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void CNTLZW(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(26, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void DCBF(int SourceReg1, int SourceReg2) {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(86, 10, 30);
	WriteIntToFile(OpHex);
}

void DCBST(int SourceReg1, int SourceReg2) {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(246, 10, 30);
	WriteIntToFile(OpHex);
}

void DIVW(int DestReg, int DividendReg, int DivisorReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(DividendReg, 5, 15);
	OpHex |= GetOpSegment(DivisorReg, 5, 20);
	OpHex |= GetOpSegment(491, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void DIVWU(int DestReg, int DividendReg, int DivisorReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(DividendReg, 5, 15);
	OpHex |= GetOpSegment(DivisorReg, 5, 20);
	OpHex |= GetOpSegment(459, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void EQV(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(284, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void EXTSB(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(954, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FABS(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(264, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FADD(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(21, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FADDS(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(59, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(21, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FCMPU(int FPReg1, int FPReg2, int CondField)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(CondField, 3, 8);
	OpHex |= GetOpSegment(FPReg1, 5, 15);
	OpHex |= GetOpSegment(FPReg2, 5, 20);
	WriteIntToFile(OpHex);
}

void FCTIW(int SourceReg, int DestReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(14, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FCTIWZ(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(15, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FDIV(int FPDestReg, int FPSourceReg1, int FPSourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(FPDestReg, 5, 10);
	OpHex |= GetOpSegment(FPSourceReg1, 5, 15);
	OpHex |= GetOpSegment(FPSourceReg2, 5, 20);
	OpHex |= GetOpSegment(18, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FDIVS(int FPDestReg, int FPSourceReg1, int FPSourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(59, 6, 5);
	OpHex |= GetOpSegment(FPDestReg, 5, 10);
	OpHex |= GetOpSegment(FPSourceReg1, 5, 15);
	OpHex |= GetOpSegment(FPSourceReg2, 5, 20);
	OpHex |= GetOpSegment(18, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FMR(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(72, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FMUL(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 25);
	OpHex |= GetOpSegment(25, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FMULS(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(59, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 25);
	OpHex |= GetOpSegment(25, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FNEG(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(40, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FRES(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(59, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(24, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

//Rounds a 64-bit, double precision floating-point operand to single precision
//places result in a floating-point register
void FRSP(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(12, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FRSQRTE(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 20);
	OpHex |= GetOpSegment(26, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FSQRT(int FPDestReg, int FPSourceReg, bool SetConditionReg) {
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(FPDestReg, 5, 10);
	OpHex |= GetOpSegment(FPSourceReg, 5, 20);
	OpHex |= GetOpSegment(22, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FSUB(int FPDestReg, int FPSourceReg1, int FPSourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(63, 6, 5);
	OpHex |= GetOpSegment(FPDestReg, 5, 10);
	OpHex |= GetOpSegment(FPSourceReg1, 5, 15);
	OpHex |= GetOpSegment(FPSourceReg2, 5, 20);
	OpHex |= GetOpSegment(20, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void FSUBS(int FPDestReg, int FPSourceReg1, int FPSourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(59, 6, 5);
	OpHex |= GetOpSegment(FPDestReg, 5, 10);
	OpHex |= GetOpSegment(FPSourceReg1, 5, 15);
	OpHex |= GetOpSegment(FPSourceReg2, 5, 20);
	OpHex |= GetOpSegment(20, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void ICBI(int SourceReg1, int SourceReg2) {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(982, 10, 30);
	WriteIntToFile(OpHex);
}

void ISYNC() {
	OpHex = GetOpSegment(19, 6, 5);
	OpHex |= GetOpSegment(150, 10, 30);
	WriteIntToFile(OpHex);
}

void LBA(int DestReg, int AddressReg, int Immediate)
{
	LBZ(DestReg, AddressReg, Immediate);
	EXTSB(DestReg, DestReg);
}

void LBAU(int DestReg, int AddressReg, int Immediate)
{
	LBZU(DestReg, AddressReg, Immediate);
	EXTSB(DestReg, DestReg);
}

void LBAUX(int DestReg, int AddressReg1, int AddressReg2)
{
	LBZUX(DestReg, AddressReg1, AddressReg2);
	EXTSB(DestReg, DestReg);
}

void LBAX(int DestReg, int AddressReg1, int AddressReg2)
{
	LBZX(DestReg, AddressReg1, AddressReg2);
	EXTSB(DestReg, DestReg);
}

void LBZ(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(34, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LBZU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(35, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LBZUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(119, 10, 30);
	WriteIntToFile(OpHex);
}

void LBZX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(87, 10, 30);
	WriteIntToFile(OpHex);
}

void LFD(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(50, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LFS(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(48, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LFSU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(49, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LFSUX(int DestReg, int AddressReg, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(567, 10, 30);
	WriteIntToFile(OpHex);
}

void LFSX(int DestReg, int AddressReg, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(535, 10, 30);
	WriteIntToFile(OpHex);
}

void LHZ(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(40, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LHZU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(41, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LHZUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(311, 10, 30);
	WriteIntToFile(OpHex);
}

void LHZX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(279, 10, 30);
	WriteIntToFile(OpHex);
}

void LWZ(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(32, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LWZU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(33, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LWZUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(55, 10, 30);
	WriteIntToFile(OpHex);
}

void LWZX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(23, 10, 30);
	WriteIntToFile(OpHex);
}

void LMW(int StartReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(46, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void LSWI(int StartReg, int AddressReg, int numBytes) {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(numBytes, 5, 20);
	OpHex |= GetOpSegment(597, 10, 30);
	WriteIntToFile(OpHex);
}

void LSWX(int StartReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(533, 10, 30);
	WriteIntToFile(OpHex);
}
void LSWX(int StartReg, int AddressReg1, int AddressReg2, int NumArgsReg) {
	MTXER(NumArgsReg);
	LSWX(StartReg, AddressReg1, AddressReg2);
}

void MFCTR(int TargetReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(9 << 5, 10, 20); //spr
	OpHex |= GetOpSegment(339, 10, 30);
	WriteIntToFile(OpHex);
}

void MFLR(int TargetReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(8 << 5, 10, 20); //spr
	OpHex |= GetOpSegment(339, 10, 30);
	WriteIntToFile(OpHex);
}

void MFXER(int TargetReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(1 << 5, 10, 20); //xer
	OpHex |= GetOpSegment(339, 10, 30);
	WriteIntToFile(OpHex);
}

//DestReg = SourceReg1 % SourceReg2
void MOD(int DestReg, int SourceReg1, int SourceReg2)
{
	DIVWU(DestReg, SourceReg1, SourceReg2);
	MULLW(DestReg, DestReg, SourceReg2);
	SUBF(DestReg, SourceReg1, DestReg);
}

void MR(int DestReg, int SourceReg)
{
	OR(DestReg, SourceReg, SourceReg);
}

void MTCTR(int TargetReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(9 << 5, 10, 20); //spr
	OpHex |= GetOpSegment(467, 10, 30);
	WriteIntToFile(OpHex);
}

void MTLR(int TargetReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(8 << 5, 10, 20); //lr
	OpHex |= GetOpSegment(467, 10, 30);
	WriteIntToFile(OpHex);
}

void MTXER(int TargetReg) {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(TargetReg, 5, 10);
	OpHex |= GetOpSegment(1 << 5, 10, 20); //xer
	OpHex |= GetOpSegment(467, 10, 30);
	WriteIntToFile(OpHex);
}

void MULLI(int DestReg, int SourceReg, int Immediate)
{
	if (CONFIG_ALLOW_IMPLICIT_OPTIMIZATIONS && isPowerOf2((unsigned long)Immediate))
	{
		unsigned long shiftCount = bitIndexFromButtonHex(Immediate, 1);
		RLWINM(DestReg, SourceReg, shiftCount, 0x00, 0x1F - shiftCount );
	}
	else
	{
		OpHex = GetOpSegment(7, 6, 5);
		OpHex |= GetOpSegment(DestReg, 5, 10);
		OpHex |= GetOpSegment(SourceReg, 5, 15);
		OpHex |= GetOpSegment(Immediate, 16, 31);
		WriteIntToFile(OpHex);
	}
}

void MULLW(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg1, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(235, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void NEG(int DestReg, int SourceReg, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg, 5, 15);
	OpHex |= GetOpSegment(104, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void NOP()
{
	ORI(0, 0, 0);
}

void NOR(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(124, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void OR(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(444, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void ORC(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(412, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void ORI(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(24, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void ORIS(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(25, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void RLWIMI(int DestReg, int SourceReg, int ShiftNum, int MaskStart, int MaskEnd, bool SetConditionReg)
{
	OpHex = GetOpSegment(20, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(ShiftNum, 5, 20);
	OpHex |= GetOpSegment(MaskStart, 5, 25);
	OpHex |= GetOpSegment(MaskEnd, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void RLWINM(int DestReg, int SourceReg, int ShiftNum, int MaskStart, int MaskEnd, bool SetConditionReg)
{
	OpHex = GetOpSegment(21, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(ShiftNum, 5, 20);
	OpHex |= GetOpSegment(MaskStart, 5, 25);
	OpHex |= GetOpSegment(MaskEnd, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void RLWNM(int DestReg, int SourceReg, int ShiftReg, int MaskStart, int MaskEnd, bool SetConditionReg)
{
	OpHex = GetOpSegment(23, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(ShiftReg, 5, 20);
	OpHex |= GetOpSegment(MaskStart, 5, 25);
	OpHex |= GetOpSegment(MaskEnd, 5, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void SRAWI(int DestReg, int SourceReg, int ShiftNum, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(ShiftNum, 5, 20);
	OpHex |= GetOpSegment(824, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void STB(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(38, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STBU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(39, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STBUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(247, 10, 30);
	WriteIntToFile(OpHex);
}

void STBX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(215, 10, 30);
	WriteIntToFile(OpHex);
}

void STFD(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(54, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STFS(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(52, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STFSU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(53, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STH(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(44, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STHU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(45, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STHUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(439, 10, 30);
	WriteIntToFile(OpHex);
}

void STHX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(407, 10, 30);
	WriteIntToFile(OpHex);
}

void STMW(int StartReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(47, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STSWI(int StartReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 5, 20);
	OpHex |= GetOpSegment(725, 10, 30);
	WriteIntToFile(OpHex);
}

void STSWX(int StartReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(StartReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(661, 10, 30);
	WriteIntToFile(OpHex);
}
void STSWX(int StartReg, int AddressReg1, int AddressReg2, int NumArgsReg)
{
	MTXER(NumArgsReg);
	STSWX(StartReg, AddressReg1, AddressReg2);
}

void STW(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(36, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STWU(int DestReg, int AddressReg, int Immediate)
{
	OpHex = GetOpSegment(37, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void STWUX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(183, 10, 30);
	WriteIntToFile(OpHex);
}

void STWX(int DestReg, int AddressReg1, int AddressReg2)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(AddressReg1, 5, 15);
	OpHex |= GetOpSegment(AddressReg2, 5, 20);
	OpHex |= GetOpSegment(151, 10, 30);
	WriteIntToFile(OpHex);
}

//DestReg = SourceReg1 - SourceReg2
void SUBF(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(DestReg, 5, 10);
	OpHex |= GetOpSegment(SourceReg2, 5, 15);
	OpHex |= GetOpSegment(SourceReg1, 5, 20);
	OpHex |= GetOpSegment(40, 9, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void SYNC() {
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(598, 10, 30);
	WriteIntToFile(OpHex);
}

void XOR(int DestReg, int SourceReg1, int SourceReg2, bool SetConditionReg)
{
	OpHex = GetOpSegment(31, 6, 5);
	OpHex |= GetOpSegment(SourceReg1, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(SourceReg2, 5, 20);
	OpHex |= GetOpSegment(316, 10, 30);
	OpHex |= GetOpSegment(SetConditionReg, 1, 31);
	WriteIntToFile(OpHex);
}

void XORI(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(26, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}

void XORIS(int DestReg, int SourceReg, int Immediate)
{
	OpHex = GetOpSegment(27, 6, 5);
	OpHex |= GetOpSegment(SourceReg, 5, 10);
	OpHex |= GetOpSegment(DestReg, 5, 15);
	OpHex |= GetOpSegment(Immediate, 16, 31);
	WriteIntToFile(OpHex);
}