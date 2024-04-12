#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"
#include <Engine/CFontMgr.h>

// 연습 모드 매니저
// 결과적으로 연습 모드는, 빈 오브젝트에 연습 모드 관리자 스크립트를 부착하고 시작할 경우
// 정상적으로 연습 모드가 동작될 수 있도록 만들자.
class CPracticeModeMgr :
    public CScript
{
private:
    /// 필드
    class CFieldScript*         m_Field;
    class CSummonDiceBtnScript* m_SummonBtn;
    class CSPBarScript*         m_SPBar;
    class CPlayerHP*            m_HP;

    // 웨이브
    UINT                        m_CurWave = 1;
    float                       m_MaxWaveTime = 45.f;
    float                       m_CurWaveTime;

    FONTINFO                    m_WaveFontInfo;

    // 보스
    bool                        m_IsBossAppears;
    
    BOSS_TYPE                   m_CurBoss;

    float                       m_TempBossTimer; // 나중에 바꿔주어야 한다...!

    // 라운드
    class CWaveTimer*           m_WaveTimer;


public:
    CFieldScript* GetField()
    {
        return m_Field;
    }

    bool IsBossAppears() const { return m_IsBossAppears; }
    float GetCurWaveTime() const { return m_CurWaveTime; }

private:
    void BossAppears();
    void WaveEnd();

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CPracticeModeMgr);
    CPracticeModeMgr();
    CPracticeModeMgr(const CPracticeModeMgr& _Origin);
    ~CPracticeModeMgr();
};

