#include "pch.h"
#include "CDiceScript.h"

CDiceScript::CDiceScript()
	:CScript(DICESCRIPT)
{

	// �ֻ��� ����
	AddScriptParam(SCRIPT_PARAM::INT, "Dice ", &Dice);

	// ��� ���� �ֻ��� ��ġ
	AddScriptParam(SCRIPT_PARAM::INT, "DiceRow :", &m_DiceRow);
	AddScriptParam(SCRIPT_PARAM::INT, "DiceCol :", &m_DiceCol);

	// ���� ����(���)
	AddScriptParam(SCRIPT_PARAM::INT, "Grade :", &m_DiceGrade);



}

CDiceScript::~CDiceScript()
{


}
