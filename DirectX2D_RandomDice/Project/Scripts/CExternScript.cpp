#include "pch.h"
#include "CExternScript.h"

CExternScript::CExternScript()
	:CScript(SCRIPT_TYPE::EXTERNSCRIPT)
{
	// 시작 덱을 전부 바람 주사위로 초기화
	m_diceDeck.resize(5, (UINT)DICE::WIND);

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