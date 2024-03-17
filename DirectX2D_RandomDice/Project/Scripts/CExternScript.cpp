#include "pch.h"
#include "CExternScript.h"

CExternScript::CExternScript()
	:CScript(SCRIPT_TYPE::EXTERNSCRIPT)
{
	// ���� ���� ���� �ٶ� �ֻ����� �ʱ�ȭ
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

	// ���� ���� �ִ� �ֻ����� ��� ������ ��ġ�� �ٲ��־�� �Ѵ�.
	for (int i = 0; i < 5; ++i)
		if ((UINT)_diceType == m_diceDeck[_idx])
			duplicateDiceIdx = _idx;
	
	// ���� ���� �ֻ����� ���� ���
	if (duplicateDiceIdx == -1) 
		m_diceDeck[_idx] = (UINT)_diceType;

	// ���� ���� �ֻ����� ���� ���
	else			
	{
		int Temp = m_diceDeck[_idx];
		m_diceDeck[_idx] = m_diceDeck[duplicateDiceIdx];
		m_diceDeck[duplicateDiceIdx] = Temp;
	}
}