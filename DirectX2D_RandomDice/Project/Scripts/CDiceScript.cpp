#include "pch.h"
#include "CDiceScript.h"

CDiceScript::CDiceScript()
	:CScript(DICESCRIPT)
{

	// 주사위 종류
	AddScriptParam(SCRIPT_PARAM::INT, "Dice ", &Dice);

	// 행렬 상의 주사위 위치
	AddScriptParam(SCRIPT_PARAM::INT, "DiceRow :", &m_DiceRow);
	AddScriptParam(SCRIPT_PARAM::INT, "DiceCol :", &m_DiceCol);

	// 눈금 개수(등급)
	AddScriptParam(SCRIPT_PARAM::INT, "Grade :", &m_DiceGrade);



}

CDiceScript::~CDiceScript()
{


}
