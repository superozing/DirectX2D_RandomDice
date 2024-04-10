#pragma once
#include <Engine\CScript.h>
#include "ScriptDefine.h"
#include <random>

struct SPAWN_ENEMY_COOLDOWN
{
    UINT    EnemySpawnCount;
    float   CoolDown;

public:
    bool EnableSpawn() const { return CoolDown == 0.f; }
};


// �ʵ��� �ֻ��� ��ġ�� �ֻ��� �Լ� ȣ���� ������ �ʵ� ��ũ��Ʈ
class CFieldScript :
    public CScript
{
private:

    enum class DICE         m_Deck[5]; // �迭�ε� �� �𸣳�->Dice�� ����.
    UINT                    m_DiceLevel[5] = { 1, 1, 1, 1, 1 }; // ���� ���� �ִ� �ֻ������� ����
    UINT                    m_LevelUpSP[5] = { 100, 200, 400, 700, 0 }; // �������� ��� SP ���

    UINT                    m_CurWave;

    class CDiceScript*      m_DiceField[5][3]; // CDiceScript�� ���� CGameObject�� 2���� �迭

    // ���� ��Ŀ��
    CDiceScript*            m_FocusDice;
    CGameObject*            m_FocusMarker;

    UINT                    m_CurDiceCount;

    list<ENEMY_PAIR>        m_EnemyList;

    // ���� �켱 ������ �ش��ϴ� �� ������ ����
    ENEMY_PAIR              m_AttackPriority[(UINT)ATTACK_PRIORITY::END];

    // �� ������
    Ptr<CPrefab>            m_EnemyPrefab[(UINT)ENEMY_TYPE::END];
    SPAWN_ENEMY_COOLDOWN    m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::END];

    float                   m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::END];



    int                     m_SP;

    UINT                    m_SummonSP;


    // �� ü�� ����
    int                     m_MaxEnemyHP;
    float                   m_EnemyHPUpdateTimer;

    // ���忡 ���� �޶����� �� ��
    float                   m_EnemySpawnRate[10] = { 0.f, 0.7f, 1.f, 1.f, 1.2f, 1.5f, 1.7f, 2.f, 2.2f, 2.5f };
    int                     m_EnemyHPArr[10] = { 0, 100, 1000, 2000, 5000, 9000, 25000, 50000, 100000, 200000 };

    // �÷��̾� HP
    int                     m_PlayerHP = 3;

    // Debug
    bool                    AutoSpawnEnemy;
    int                     IsInvincible;

    // random
    std::random_device      m_rd;
    std::mt19937            m_gen;
    std::uniform_int_distribution<UINT> m_XDis;
    std::uniform_int_distribution<UINT> m_YDis;

private: // EnemyPos ��� �뵵�� GameObject*
    CGameObject* m_EnemyGate1;
    CGameObject* m_Line2;
    CGameObject* m_EnemyGate2;

    class CDiceScript* GetRandomFieldDice() 
    {
        // 0 ~ 4, 0 ~ 2
        return m_DiceField[m_XDis(m_gen)][m_YDis(m_gen)];
    }


public:
    virtual void begin() override;
    virtual void tick() override;

    ENEMY_PAIR GetTargetEnemy(ATTACK_PRIORITY attack_priority) const { return m_AttackPriority[(UINT)attack_priority]; }

    list<ENEMY_PAIR>& GetEnemyList() { return m_EnemyList; }

    UINT GetSummonSP() const { return m_SummonSP; }

    UINT GetCurWave() const { return m_CurWave; }

    UINT GetDiceCount() const { return m_CurDiceCount; }

    DICE GetRandomDeckDiceType()
    {
        return m_Deck[m_XDis(m_gen)];
    }

    void AddCurSP(int _addSP) { m_SP += _addSP; }
    int  GetCurSP() const { return m_SP; }
    
    int GetPlayerHP() const { return m_PlayerHP; }

    void SetPlayerHP(int _HP) 
    { 
        if (IsInvincible)
            return;

        m_PlayerHP = _HP; 

        if (m_PlayerHP < 0)
            m_PlayerHP = 0;
    }

    void SetFocusDice(CDiceScript* _Dice);

    void SetDiceMergeState();

    CDiceScript* GetFocusDice()
    {
        return m_FocusDice;
    }


public:

    // �ֻ��� ��ȯ �Լ�
    void SummonDice();
    void SpawnDice(UINT _LOW, UINT _COL);

    // �ֻ��� ���� ��(��ȯ���� "���� ������ ���")
    UINT DiceLevelUp(DICE _dice, UINT _idx);

    // ���� �ֻ����� ������ ��ȯ����. ���� �ش� �ֻ����� ������ ���� ��� 0�� ��ȯ�ؿ�.
    UINT GetCurDiceLevel(DICE _dice) const;

private:
    void LoadDeckInfoFromFile();
    void SaveDeckInfoFromFile();



public:
    virtual void SaveToFile(FILE* _File){}
    virtual void LoadFromFile(FILE* _File){}

public:
    CLONE_DISABLE(CFieldScript);
    CFieldScript();
    ~CFieldScript();

};

