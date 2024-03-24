#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>

class CSummonDiceBtnScript :
	public CScript
{
private:

	class CFieldScript* m_pFieldScript;

	class CUIScript*	m_UI;

	// ��ȯ �� �Ҹ� SP ���
	FONTINFO            m_fInfo1;
	FONTINFO            m_fInfo2;

	// ���� ũ��
	Vec3				m_vScale;
	float				m_fScaleSize;

public:
	virtual void begin() override;
	virtual void tick() override;


	void ClickButton();


public:
	virtual void SaveToFile(FILE* _File) {}
	virtual void LoadFromFile(FILE* _File) {}

public:
	CLONE(CSummonDiceBtnScript);
	CSummonDiceBtnScript();
	CSummonDiceBtnScript(const CSummonDiceBtnScript& _Origin);
	~CSummonDiceBtnScript();
};

