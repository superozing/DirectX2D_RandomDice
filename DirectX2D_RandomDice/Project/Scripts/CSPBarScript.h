#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>

// SP ǥ���� SPBar Script
class CSPBarScript :
	public CScript
{
private:

	class CFieldScript* m_pFieldScript;

	// ���� ������ SP ���
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
