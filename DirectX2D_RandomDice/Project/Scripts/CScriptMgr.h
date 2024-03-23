#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BACKGROUNDSCRIPT,
	BIGENEMYSCRIPT,
	DEFAULTENEMYSCRIPT,
	DICEATTACKSCRIPT,
	DICEBUFFSCRIPT,
	DICEMERGESCRIPT,
	DICESCALESCRIPT,
	DICESCRIPT,
	ENEMYGATESCRIPT,
	ENEMYSCRIPT,
	EXTERNSCRIPT,
	FIELDSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLAYERSCRIPT,
	ROTATESCRIPT,
	SPEEDENEMYSCRIPT,
	SUMMONDICEBTNSCRIPT,
	UISCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
