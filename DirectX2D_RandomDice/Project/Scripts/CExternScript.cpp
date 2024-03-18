#include "pch.h"
#include "CExternScript.h"

CExternScript::CExternScript()
	:CScript(SCRIPT_TYPE::EXTERNSCRIPT)
{
	DicePath.resize((UINT)DICE::END);

	DicePath[(UINT)DICE::NONE]		= L"NONE";

	DicePath[(UINT)DICE::FIRE]		= L"texture\\Dice\\01_fire.png";
	DicePath[(UINT)DICE::ELECTRIC]	= L"texture\\Dice\\02_electric.png";
	DicePath[(UINT)DICE::WIND]		= L"texture\\Dice\\03_wind.png";
	DicePath[(UINT)DICE::POISON]	= L"texture\\Dice\\04_poison.png";
	DicePath[(UINT)DICE::ICE]		= L"texture\\Dice\\05_ice.png";
	DicePath[(UINT)DICE::IRON]		= L"texture\\Dice\\06_iron.png";
	DicePath[(UINT)DICE::BROKEN]	= L"texture\\Dice\\07_broken.png";
	DicePath[(UINT)DICE::GAMBLE]	= L"texture\\Dice\\08_gamble.png";
	DicePath[(UINT)DICE::LOCK]		= L"texture\\Dice\\09_lock.png";
	DicePath[(UINT)DICE::MINE]		= L"texture\\Dice\\10_mine.png";
	DicePath[(UINT)DICE::LIGHT]		= L"texture\\Dice\\11_light.png";
	DicePath[(UINT)DICE::THORN]		= L"texture\\Dice\\12_thorn.png";
	DicePath[(UINT)DICE::CRACK]		= L"texture\\Dice\\13_crack.png";
	DicePath[(UINT)DICE::CRITICAL]	= L"texture\\Dice\\14_critical.png";
	DicePath[(UINT)DICE::ENERGY]	= L"texture\\Dice\\15_energy.png";
	DicePath[(UINT)DICE::SACRIFICE]	= L"texture\\Dice\\16_sacrifice.png";
	DicePath[(UINT)DICE::ARROW]		= L"texture\\Dice\\17_arrow.png";
	DicePath[(UINT)DICE::MIMIC]		= L"texture\\Dice\\18_mimic.png";

	///////////////////////////////////////////////////////


	DiceInfo.resize((UINT)DICE::END);

	DICE_INFO tDiceInfo;

	//////////////
	// 01_fire
	//////////////










	// 시작 덱을 전부 바람 주사위로 초기화
	m_diceDeck.resize(5);

	AddScriptParam(SCRIPT_PARAM::INT, "DICE 1 ", &m_diceDeck[0]);
	AddScriptParam(SCRIPT_PARAM::INT, "DICE 2 ", &m_diceDeck[1]);
	AddScriptParam(SCRIPT_PARAM::INT, "DICE 3 ", &m_diceDeck[2]);
	AddScriptParam(SCRIPT_PARAM::INT, "DICE 4 ", &m_diceDeck[3]);
	AddScriptParam(SCRIPT_PARAM::INT, "DICE 5 ", &m_diceDeck[4]);


}

CExternScript::~CExternScript()
{
}

void CExternScript::begin()
{

}

void CExternScript::tick()
{
}

void CExternScript::SaveToFile(FILE* _File)
{
}

void CExternScript::LoadFromFile(FILE* _File)
{
}

void CExternScript::SetDice(UINT _idx, DICE _diceType)
{
	int duplicateDiceIdx = -1;

	// 기존 덱에 있는 주사위일 경우 서로의 위치를 바꿔주어야 한다.
	for (int i = 0; i < 5; ++i)
		if ((UINT)_diceType == m_diceDeck[_idx])
			duplicateDiceIdx = _idx;
	
	// 덱에 같은 주사위가 없을 경우
	if (duplicateDiceIdx == -1) 
		m_diceDeck[_idx] = (UINT)_diceType;

	// 덱에 같은 주사위가 있을 경우
	else			
	{
		int Temp = m_diceDeck[_idx];
		m_diceDeck[_idx] = m_diceDeck[duplicateDiceIdx];
		m_diceDeck[duplicateDiceIdx] = Temp;
	}
}