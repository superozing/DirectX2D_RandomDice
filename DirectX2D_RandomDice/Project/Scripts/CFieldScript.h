#pragma once
#include <Engine\CScript.h>
#include "CDiceAttackScript.h"
#include "CEnemyScript.h"

// EnemyScript�� ���� �� ������ �����ϱ� ���ؼ� ����ü ���
struct ENEMY_PAIR
{
    CGameObject*            pObject;
    class CEnemyScript*     pEnemyScript;
};


// �ʵ��� �ֻ��� ��ġ�� �ֻ��� �Լ� ȣ���� ������ �ʵ� ��ũ��Ʈ
class CFieldScript :
    public CScript
{
private:
    class CDiceScript*  m_DiceField[5][3]; // CDiceScript�� ���� CGameObject�� 2���� �迭

    list<ENEMY_PAIR>    m_EnemyList;

    // ���� �켱 ������ �ش��ϴ� �� ������ ����
    ENEMY_PAIR          m_AttackPriority[(UINT)ATTACK_PRIORITY::END];

    // �� ������
    Ptr<CPrefab>        m_EnemyPrefab[(UINT)ENEMY_TYPE::END];
    bool                m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::END];



private: // EnemyPos ��� �뵵�� GameObject*
    CGameObject* m_EnemyGate1;
    CGameObject* m_Line2;
    CGameObject* m_EnemyGate2;




public:
    virtual void begin() override;
    virtual void tick() override;

    ENEMY_PAIR GetTargetEnemy(ATTACK_PRIORITY attack_priority) const { return m_AttackPriority[(UINT)attack_priority]; }
public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};

