#pragma once
#include <Engine\CScript.h>
#include <Engine\CFontMgr.h>
#include "CDiceScript.h"

class CDiceUI :
    public CScript
{
private:

    class CFieldScript* m_pField;

    DICE                m_Dice; // ����� �ֻ��� ����

    UINT                m_DeckIdx;

    class CUIScript*    m_pUI;

    FONTINFO            m_FSP1;
    FONTINFO            m_FSP2;

    FONTINFO            m_FDiceEyeCount;
    
    FONTINFO            m_FDiceLevel;

    CMeshRender*        m_DiceEyeBG;
    CMeshRender*        m_SPImg;

    // Ŭ�� �� ũ�� ����
    // ���� ũ��
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


