#pragma once


#define TEX_LOAD(wstrPath) CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath)
#define ASSET_MGR CAssetMgr::GetInst()

enum class DICE_MERGE_STATE
{
    NOT_SET,
    ABLE,
    UNABLE,
};


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
class CDiceMergeScript;

typedef  void(CDiceAttackScript::* EnemyAttackDelegate)(struct ENEMY_PAIR);
typedef  void(CDiceMergeScript::* DiceMergeDelegate)(class CDiceScript*);
typedef  bool(CDiceMergeScript::* IsMergeAbleDelegate)(class CDiceScript*);


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