#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"

// 연습 모드 매니저
// 결과적으로 연습 모드는, 빈 오브젝트에 연습 모드 관리자 스크립트를 부착하고 시작할 경우
// 정상적으로 연습 모드가 동작될 수 있도록 만들자.
class CPracticeModeMgr :
    public CScript
{
private:
    /// 기존 필드 종속 오브젝트
    // 1. 필드
    class CFieldScript*         m_Field;

    // 2. 주사위 소환 버튼
    class CSummonDiceBtnScript* m_SummonBtn;

    // 3. SP 바
    class CSPBarScript*         m_SPBar;

    // 4. 플레이어 HP
    class CPlayerHP*            m_HP;

public:
    CFieldScript* GetField()
    {
        return m_Field;
    }

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

