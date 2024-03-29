#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>

// SP 표시할 SPBar Script
class CSPBarScript :
	public CScript
{
private:

	class CFieldScript* m_pFieldScript;

	// 현재 보유한 SP 출력
	FONTINFO                m_fInfo;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	virtual void SaveToFile(FILE* _File) {}
	virtual void LoadFromFile(FILE* _File) {}

public:
	CLONE(CSPBarScript);
	CSPBarScript();
	CSPBarScript(const CSPBarScript& _Origin);
	~CSPBarScript();
	 
};

