#include "pch.h"
#include "CDiceMergeScript.h"

CDiceMergeScript::CDiceMergeScript()
	:CScript(DICEMERGESCRIPT)
{
}

CDiceMergeScript::CDiceMergeScript(UINT _ScriptType)
	:CScript(_ScriptType)
{
}

CDiceMergeScript::CDiceMergeScript(const CDiceMergeScript& _Origin)
	:CScript(DICEMERGESCRIPT)
{
}

CDiceMergeScript::~CDiceMergeScript()
{
}
