#pragma once
#include <Engine\CScript.h>
#include "CDiceAttackScript.h"
#include "CEnemyScript.h"

// EnemyScript를 조금 더 빠르게 참조하기 위해서 구조체 사용
struct ENEMY_PAIR
{
    CGameObject*            pObject;
    class CEnemyScript*     pEnemyScript;
};


struct SPAWN_ENEMY_COOLDOWN
{
    UINT    EnemySpawnCount;
    float   CoolDown;

public:
    bool EnableSpawn() const { return CoolDown == 0.f; }
};


// 필드의 주사위 배치와 주사위 함수 호출을 관리할 필드 스크립트
class CFieldScript :
    public CScript
{
private:
    class CDiceScript*      m_DiceField[5][3]; // CDiceScript를 가진 CGameObject의 2차원 배열

    list<ENEMY_PAIR>        m_EnemyList;

    // 공격 우선 순위에 해당하는 적 포인터 모음
    ENEMY_PAIR              m_AttackPriority[(UINT)ATTACK_PRIORITY::END];

    // 적 프리팹
    Ptr<CPrefab>            m_EnemyPrefab[(UINT)ENEMY_TYPE::END];
    SPAWN_ENEMY_COOLDOWN    m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::END];

    float                   m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::END];

    FONTINFO                m_fInfo;

    // Debug
    bool                    AutoSpawnEnemy;





private: // EnemyPos 계산 용도의 GameObject*
    CGameObject* m_EnemyGate1;
    CGameObject* m_Line2;
    CGameObject* m_EnemyGate2;




public:
    virtual void begin() override;
    virtual void tick() override;

    ENEMY_PAIR GetTargetEnemy(ATTACK_PRIORITY attack_priority) const { return m_AttackPriority[(UINT)attack_priority]; }



public:

    // 주사위 소환 함수
    void SummonDice();
    void SpawnDice(UINT _LOW, UINT _COL);





public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};

