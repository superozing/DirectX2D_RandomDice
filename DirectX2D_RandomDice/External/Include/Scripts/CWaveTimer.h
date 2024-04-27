#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"
#include <Engine/CFontMgr.h>

class CWaveTimer :
    public CScript
{
private:
    Ptr<CTexture>       m_BossImgArr[(UINT)BOSS_TYPE::END];
    
    BOSS_TYPE           m_CurBoss;
    class CGameObject*  m_BossIcon;

    // 모드 매니져
    class CPracticeModeMgr* m_ModeMgr;


    FONTINFO            m_WaveTimeFontInfo;


public:
    void SetModeMgr(CPracticeModeMgr* _ModeMgr)
    {
        m_ModeMgr = _ModeMgr;
    }
    void SetBoss(BOSS_TYPE _Boss);


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CWaveTimer);
    CWaveTimer();
    CWaveTimer(const CWaveTimer& _Origin);
    ~CWaveTimer();
};

