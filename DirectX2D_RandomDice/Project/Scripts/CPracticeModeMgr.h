#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"
#include <Engine/CFontMgr.h>

// ���� ��� �Ŵ���
// ��������� ���� ����, �� ������Ʈ�� ���� ��� ������ ��ũ��Ʈ�� �����ϰ� ������ ���
// ���������� ���� ��尡 ���۵� �� �ֵ��� ������.
class CPracticeModeMgr :
    public CScript
{
private:
    /// �ʵ�
    class CFieldScript*         m_Field;
    class CSummonDiceBtnScript* m_SummonBtn;
    class CSPBarScript*         m_SPBar;
    class CPlayerHP*            m_HP;

    // ���̺�
    UINT                        m_CurWave = 1;
    float                       m_MaxWaveTime = 45.f;
    float                       m_CurWaveTime;

    FONTINFO                    m_WaveFontInfo;

    // ����
    bool                        m_IsBossAppears;
    
    BOSS_TYPE                   m_CurBoss;

    float                       m_TempBossTimer; // ���߿� �ٲ��־�� �Ѵ�...!

    // ����
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

