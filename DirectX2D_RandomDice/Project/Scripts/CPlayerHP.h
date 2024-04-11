#pragma once
#include <Engine\CScript.h>

class CPlayerHP :
    public CScript
{

// 무엇이 필요할까?
    // 1. 필드 스크립트 포인터
    // 2. 매쉬 렌더 컴포넌트, 각 PlayerHP에 대응하는 CTexture
private:
    Vec3                    m_PlayerHPPos;
    class CPracticeModeMgr* m_ModeMgr;

    // 필드 스크립트 포인터
    class CFieldScript* m_pField;

    // PlayerHP에 대응하는 CTexture
    Ptr<CTexture>       m_HPTex[4]; // 3, 2, 1, 0으로 각 인덱스에 알맞은 텍스쳐를 init에서 넣어주면 되겠죠?

public:
    void SetPlayerHPPos(Vec3 _PlayerHPPos)
    {
        m_PlayerHPPos = _PlayerHPPos;
    }
    void SetModeMgr(CPracticeModeMgr* _ModeMgr)
    {
        m_ModeMgr = _ModeMgr;
    }


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CPlayerHP);
    CPlayerHP();
    CPlayerHP(const CPlayerHP& _Origin);
    ~CPlayerHP();

};

