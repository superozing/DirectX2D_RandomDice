#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>

class CSummonDiceBtnScript :
	public CScript
{
private:

	Vec3 m_SummonDiceBtnPos;
	class CPracticeModeMgr* m_ModeMgr;

	class CFieldScript* m_pFieldScript;

	class CUIScript*	m_UI;

	// 소환 시 소모 SP 출력
	FONTINFO            m_fInfo1;
	FONTINFO            m_fInfo2;

	// 기존 크기
	Vec3				m_vScale;
	float				m_fScaleSize;

public:
	virtual void begin() override;
	virtual void tick() override;


	void ClickButton();
	void SetDiceBtnPos(Vec3 _BtnPos)
	{
		m_SummonDiceBtnPos = _BtnPos;
	}
	void SetModeMgr(CPracticeModeMgr* _SetModeMgr)
	{
		m_ModeMgr = _SetModeMgr;
	}
public:
	virtual void SaveToFile(FILE* _File) {}
	virtual void LoadFromFile(FILE* _File) {}

public:
	CLONE(CSummonDiceBtnScript);
	CSummonDiceBtnScript();
	CSummonDiceBtnScript(const CSummonDiceBtnScript& _Origin);
	~CSummonDiceBtnScript();
};

