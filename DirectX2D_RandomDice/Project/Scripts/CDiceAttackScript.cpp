#include "pch.h"
#include "CDiceAttackScript.h"

CDiceAttackScript::CDiceAttackScript()
	:CScript(DICEATTACKSCRIPT)
{
}

CDiceAttackScript::CDiceAttackScript(UINT _ScriptType)
	:CScript(_ScriptType)
{
}

CDiceAttackScript::CDiceAttackScript(const CDiceAttackScript& _Origin)
	:CScript(_Origin.GetScriptType())
{
}

CDiceAttackScript::~CDiceAttackScript()
{
}
