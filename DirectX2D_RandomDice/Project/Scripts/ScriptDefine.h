#pragma once


enum class ENEMY_TYPE
{
    DEFAULT,
    SPEED,
    BIG,
    BOSS,
    END
};

// EnemyScript�� ���� �� ������ �����ϱ� ���ؼ� ����ü ���
struct ENEMY_PAIR
{
    class CGameObject* pObject;
    class CEnemyScript* pEnemyScript;
};

// Functor
struct CompareEnemyPair
{
    bool operator()(const ENEMY_PAIR& enemy1, const ENEMY_PAIR& enemy2) const
    {
        // �������� ���� ��� ������ �����Ͽ� ��
        return enemy1.pEnemyScript->GetMoveProgress() < enemy2.pEnemyScript->GetMoveProgress();
    }
};

enum class ATTACK_PRIORITY // �ֻ����� ���� �켱 ����
{
    BUFF,		 // ���� �ֻ��� - ������ ���� ����
    FRONT,		 // �� �� �� �켱 ����
    HIGH_HEALTH, // ���� ü���� �� �켱 ����
    RANDOM,      // ������ �� ����
    RANGE_ATTACK,// ������ ���� ����
    END,
};
class CDiceAttackScript;

typedef  void(CDiceAttackScript::* EnemyAttackDelegate)(struct ENEMY_PAIR);


enum class DICE
{
    NONE,

    FIRE,
    ELECTRIC,
    WIND,
    POISON,
    ICE,

    IRON,
    BROKEN,
    GAMBLE,
    LOCK,
    MINE,

    LIGHT,
    THORN,
    CRACK,
    CRITICAL,
    ENERGY,

    SACRIFICE,
    ARROW,
    MIMIC,

    END,
};



enum class DICE_BUFF
{
    NONE,
    CRITICAL,
    ATTACK_SPEED,
};


struct DICE_INFO
{
    class CDiceAttackScript* pAttack;
    class CDiceBuffScript* pBuff;
    class CDiceMergeScript* pMerge;
};