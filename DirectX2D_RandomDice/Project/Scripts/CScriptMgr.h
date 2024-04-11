#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	BIGENEMYSCRIPT,
	BUFFATTACK,
	DEFAULTATTACK,
	DEFAULTENEMYSCRIPT,
	DEFAULTMERGE,
	DICEATTACKSCRIPT,
	DICEBUFFSCRIPT,
	DICEMERGESCRIPT,
	DICESCALE,
	DICESCALEPROJECTILE,
	DICESCRIPT,
	DICEUI,
	ELECTRICATTACK,
	ENEMYGATESCRIPT,
	ENEMYSCRIPT,
	EXTERNSCRIPT,
	FIELDSCRIPT,
	PLAYERHP,
	PRACTICEMODEMGR,
	ROTATESCRIPT,
	SPBARSCRIPT,
	SPEEDENEMYSCRIPT,
	SUMMONDICEBTNSCRIPT,
	UISCRIPT,
	WINDATTACK,
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
