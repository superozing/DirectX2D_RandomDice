#include "pch.h"
#include "CBossScript.h"

#include "CFieldScript.h"

CBossScript::CBossScript()
	:CEnemyScript(BOSSSCRIPT)
{
}

CBossScript::CBossScript(SCRIPT_TYPE _ScriptType)
	:CEnemyScript(_ScriptType)
{
}

CBossScript::CBossScript(const CBossScript& _Origin)
	:CEnemyScript((SCRIPT_TYPE)_Origin.GetScriptType())
{
}

CBossScript::~CBossScript()
{
}

void CBossScript::SetDeadEnemy()
{
	CEnemyScript::SetDeadEnemy();

	// 보스 사망 시, SetBossAlive를 false로 막아준다.
	GetField()->SetBossAlive(false);
}