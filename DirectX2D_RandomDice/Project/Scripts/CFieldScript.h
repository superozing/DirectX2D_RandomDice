#pragma once
#include <Engine\CScript.h>
#include "CDiceAttackScript.h"
#include "CEnemyScript.h"
#include <random>

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

    UINT                    m_CurWave;

    class CDiceScript*      m_DiceField[5][3]; // CDiceScript를 가진 CGameObject의 2차원 배열

    UINT                    m_CurDiceCount;

    list<ENEMY_PAIR>        m_EnemyList;

    // 공격 우선 순위에 해당하는 적 포인터 모음
    ENEMY_PAIR              m_AttackPriority[(UINT)ATTACK_PRIORITY::END];

    // 적 프리팹
    Ptr<CPrefab>            m_EnemyPrefab[(UINT)ENEMY_TYPE::END];
    SPAWN_ENEMY_COOLDOWN    m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::END];

    float                   m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::END];


    FONTINFO                m_fInfo1;
    FONTINFO                m_fInfo2;



    int                     m_SP;

    UINT                    m_SummonSP;


    // 적 체력 관련
    int                     m_MaxEnemyHP;
    float                   m_EnemyHPUpdateTimer;

    // 라운드에 따라 달라지는 값 들
    float                   m_EnemySpawnRate[10] = { 0, 0.7, 1, 1, 1.2, 1.5, 1.7, 2, 2.2, 2.5 };
    int                     m_EnemyHPArr[10] = { 0, 100, 1000, 2000, 5000, 9000, 25000, 50000, 100000, 200000 };

    // Debug
    bool                    AutoSpawnEnemy;


    // random
    std::random_device      m_rd;
    std::mt19937            m_gen;
    std::uniform_int_distribution<UINT> m_XDis;
    std::uniform_int_distribution<UINT> m_YDis;

private: // EnemyPos 계산 용도의 GameObject*
    CGameObject* m_EnemyGate1;
    CGameObject* m_Line2;
    CGameObject* m_EnemyGate2;

    CDiceScript* GetRandomDice() 
    {
        // 0 ~ 4, 0 ~ 2
        return m_DiceField[m_XDis(m_gen)][m_YDis(m_gen)];
    }


public:
    virtual void begin() override;
    virtual void tick() override;

    ENEMY_PAIR GetTargetEnemy(ATTACK_PRIORITY attack_priority) const { return m_AttackPriority[(UINT)attack_priority]; }

    UINT GetSummonSP() const { return m_SummonSP; }

    UINT GetCurWave() const { return m_CurWave; }


    void AddCurSP(int _addSP) { m_SP += _addSP; }
    int  GetCurSP() const { return m_SP; }
    


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

