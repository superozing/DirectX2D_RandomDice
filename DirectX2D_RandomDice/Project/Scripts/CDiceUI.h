#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>
#include "CDiceScript.h"

class CDiceUI :
    public CScript
{
private:

    class CFieldScript* m_pField;

    DICE                m_Dice; // 출력할 주사위 정보

    UINT                m_DeckIdx;

    class CUIScript*    m_pUI;

    FONTINFO            m_FSP1;
    FONTINFO            m_FSP2;

    FONTINFO            m_FDiceEyeCount;
    
    FONTINFO            m_FDiceLevel;

    CMeshRender*        m_DiceEyeBG;
    CMeshRender*        m_SPImg;

    // 클릭 시 크기 조절
    // 기존 크기
    Vec3				m_vScale;
    float				m_fScaleSize;

    
private:
    void ClickLevelUp();
    
public:
    virtual void begin() override;
    virtual void tick() override;
    
    void SetDice(DICE _dice) { m_Dice = _dice; }
    void SetDeckIdx(UINT _idx) { m_DeckIdx = _idx; }

    void SetField(CFieldScript* _pField) { m_pField = _pField; }
    
public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

    CLONE(CDiceUI);
    CDiceUI();
    CDiceUI(const CDiceUI& _Origin);
    ~CDiceUI();
};


