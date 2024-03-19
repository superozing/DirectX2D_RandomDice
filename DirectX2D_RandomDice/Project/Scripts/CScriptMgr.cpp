#include "pch.h"
#include "CScriptMgr.h"

#include "CBackgroundScript.h"
#include "CDiceAttackScript.h"
#include "CDiceBuffScript.h"
#include "CDiceMergeScript.h"
#include "CDiceScript.h"
#include "CEnemyScript.h"
#include "CExternScript.h"
#include "CFieldScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CDiceAttackScript");
	_vec.push_back(L"CDiceBuffScript");
	_vec.push_back(L"CDiceMergeScript");
	_vec.push_back(L"CDiceScript");
	_vec.push_back(L"CEnemyScript");
	_vec.push_back(L"CExternScript");
	_vec.push_back(L"CFieldScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBackgroundScript" == _strScriptName)
		return new CBackgroundScript;
	if (L"CDiceAttackScript" == _strScriptName)
		return new CDiceAttackScript;
	if (L"CDiceBuffScript" == _strScriptName)
		return new CDiceBuffScript;
	if (L"CDiceMergeScript" == _strScriptName)
		return new CDiceMergeScript;
	if (L"CDiceScript" == _strScriptName)
		return new CDiceScript;
	if (L"CEnemyScript" == _strScriptName)
		return new CEnemyScript;
	if (L"CExternScript" == _strScriptName)
		return new CExternScript;
	if (L"CFieldScript" == _strScriptName)
		return new CFieldScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return new CBackgroundScript;
		break;
	case (UINT)SCRIPT_TYPE::DICEATTACKSCRIPT:
		return new CDiceAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::DICEBUFFSCRIPT:
		return new CDiceBuffScript;
		break;
	case (UINT)SCRIPT_TYPE::DICEMERGESCRIPT:
		return new CDiceMergeScript;
		break;
	case (UINT)SCRIPT_TYPE::DICESCRIPT:
		return new CDiceScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSCRIPT:
		return new CEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::EXTERNSCRIPT:
		return new CExternScript;
		break;
	case (UINT)SCRIPT_TYPE::FIELDSCRIPT:
		return new CFieldScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BACKGROUNDSCRIPT:
		return L"CBackgroundScript";
		break;

	case SCRIPT_TYPE::DICEATTACKSCRIPT:
		return L"CDiceAttackScript";
		break;

	case SCRIPT_TYPE::DICEBUFFSCRIPT:
		return L"CDiceBuffScript";
		break;

	case SCRIPT_TYPE::DICEMERGESCRIPT:
		return L"CDiceMergeScript";
		break;

	case SCRIPT_TYPE::DICESCRIPT:
		return L"CDiceScript";
		break;

	case SCRIPT_TYPE::ENEMYSCRIPT:
		return L"CEnemyScript";
		break;

	case SCRIPT_TYPE::EXTERNSCRIPT:
		return L"CExternScript";
		break;

	case SCRIPT_TYPE::FIELDSCRIPT:
		return L"CFieldScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	}
	return nullptr;
}