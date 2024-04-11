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

// EnemyScript를 조금 더 빠르게 참조하기 위해서 구조체 사용
struct ENEMY_PAIR
{
    class CGameObject* pObject;
    class CEnemyScript* pEnemyScript;
};


enum class ATTACK_PRIORITY // 주사위의 공격 우선 순위
{
    BUFF,		 // 버프 주사위 - 공격을 하지 않음
    FRONT,		 // 앞 쪽 적 우선 공격
    HIGH_HEALTH, // 높은 체력의 적 우선 공격
    RANDOM,      // 무작위 적 공격
    RANGE_ATTACK,// 무작위 범위 공격
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