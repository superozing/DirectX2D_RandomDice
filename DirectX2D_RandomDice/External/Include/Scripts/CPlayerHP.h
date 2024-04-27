#pragma once
#include <Engine\CScript.h>

class CPlayerHP :
    public CScript
{

// ������ �ʿ��ұ�?
    // 1. �ʵ� ��ũ��Ʈ ������
    // 2. �Ž� ���� ������Ʈ, �� PlayerHP�� �����ϴ� CTexture
private:
    Vec3                    m_PlayerHPPos;
    class CPracticeModeMgr* m_ModeMgr;

    // �ʵ� ��ũ��Ʈ ������
    class CFieldScript* m_pField;

    // PlayerHP�� �����ϴ� CTexture
    Ptr<CTexture>       m_HPTex[4]; // 3, 2, 1, 0���� �� �ε����� �˸��� �ؽ��ĸ� init���� �־��ָ� �ǰ���?

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

