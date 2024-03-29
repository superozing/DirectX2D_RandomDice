#include "pch.h"
#include "CScriptMgr.h"

#include "CBackgroundScript.h"
#include "CBigEnemyScript.h"
#include "CDefaultEnemyScript.h"
#include "CDiceAttackScript.h"
#include "CDiceBuffScript.h"
#include "CDiceMergeScript.h"
#include "CDiceScaleScript.h"
#include "CDiceScript.h"
#include "CDiceUI.h"
#include "CEnemyGateScript.h"
#include "CEnemyScript.h"
#include "CExternScript.h"
#include "CFieldScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlayerHP.h"
#include "CPlayerScript.h"
#include "CRotateScript.h"
#include "CSPBarScript.h"
#include "CSpeedEnemyScript.h"
#include "CSummonDiceBtnScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBackgroundScript");
	_vec.push_back(L"CBigEnemyScript");
	_vec.push_back(L"CDefaultEnemyScript");
	_vec.push_back(L"CDiceAttackScript");
	_vec.push_back(L"CDiceBuffScript");
	_vec.push_back(L"CDiceMergeScript");
	_vec.push_back(L"CDiceScaleScript");
	_vec.push_back(L"CDiceScript");
	_vec.push_back(L"CDiceUI");
	_vec.push_back(L"CEnemyGateScript");
	_vec.push_back(L"CEnemyScript");
	_vec.push_back(L"CExternScript");
	_vec.push_back(L"CFieldScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerHP");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRotateScript");
	_vec.push_back(L"CSPBarScript");
	_vec.push_back(L"CSpeedEnemyScript");
	_vec.push_back(L"CSummonDiceBtnScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBackgroundScript" == _strScriptName)
		return new CBackgroundScript;
	if (L"CBigEnemyScript" == _strScriptName)
		return new CBigEnemyScript;
	if (L"CDefaultEnemyScript" == _strScriptName)
		return new CDefaultEnemyScript;
	if (L"CDiceAttackScript" == _strScriptName)
		return new CDiceAttackScript;
	if (L"CDiceBuffScript" == _strScriptName)
		return new CDiceBuffScript;
	if (L"CDiceMergeScript" == _strScriptName)
		return new CDiceMergeScript;
	if (L"CDiceScaleScript" == _strScriptName)
		return new CDiceScaleScript;
	if (L"CDiceScript" == _strScriptName)
		return new CDiceScript;
	if (L"CDiceUI" == _strScriptName)
		return new CDiceUI;
	if (L"CEnemyGateScript" == _strScriptName)
		return new CEnemyGateScript;
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
	if (L"CPlayerHP" == _strScriptName)
		return new CPlayerHP;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRotateScript" == _strScriptName)
		return new CRotateScript;
	if (L"CSPBarScript" == _strScriptName)
		return new CSPBarScript;
	if (L"CSpeedEnemyScript" == _strScriptName)
		return new CSpeedEnemyScript;
	if (L"CSummonDiceBtnScript" == _strScriptName)
		return new CSummonDiceBtnScript;
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
	case (UINT)SCRIPT_TYPE::BIGENEMYSCRIPT:
		return new CBigEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::DEFAULTENEMYSCRIPT:
		return new CDefaultEnemyScript;
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
	case (UINT)SCRIPT_TYPE::DICESCALESCRIPT:
		return new CDiceScaleScript;
		break;
	case (UINT)SCRIPT_TYPE::DICESCRIPT:
		return new CDiceScript;
		break;
	case (UINT)SCRIPT_TYPE::DICEUI:
		return new CDiceUI;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYGATESCRIPT:
		return new CEnemyGateScript;
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
	case (UINT)SCRIPT_TYPE::PLAYERHP:
		return new CPlayerHP;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ROTATESCRIPT:
		return new CRotateScript;
		break;
	case (UINT)SCRIPT_TYPE::SPBARSCRIPT:
		return new CSPBarScript;
		break;
	case (UINT)SCRIPT_TYPE::SPEEDENEMYSCRIPT:
		return new CSpeedEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::SUMMONDICEBTNSCRIPT:
		return new CSummonDiceBtnScript;
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

	case SCRIPT_TYPE::BIGENEMYSCRIPT:
		return L"CBigEnemyScript";
		break;

	case SCRIPT_TYPE::DEFAULTENEMYSCRIPT:
		return L"CDefaultEnemyScript";
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

	case SCRIPT_TYPE::DICESCALESCRIPT:
		return L"CDiceScaleScript";
		break;

	case SCRIPT_TYPE::DICESCRIPT:
		return L"CDiceScript";
		break;

	case SCRIPT_TYPE::DICEUI:
		return L"CDiceUI";
		break;

	case SCRIPT_TYPE::ENEMYGATESCRIPT:
		return L"CEnemyGateScript";
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

	case SCRIPT_TYPE::PLAYERHP:
		return L"CPlayerHP";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::ROTATESCRIPT:
		return L"CRotateScript";
		break;

	case SCRIPT_TYPE::SPBARSCRIPT:
		return L"CSPBarScript";
		break;

	case SCRIPT_TYPE::SPEEDENEMYSCRIPT:
		return L"CSpeedEnemyScript";
		break;

	case SCRIPT_TYPE::SUMMONDICEBTNSCRIPT:
		return L"CSummonDiceBtnScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	}
	return nullptr;
}