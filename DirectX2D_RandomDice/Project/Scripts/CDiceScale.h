#pragma once
#include <Engine\CScript.h>
#include "CDiceScaleProjectile.h"
#include "CEnemyScript.h"
#include "CDiceScript.h"

// 주사위 눈금 스크립트
class CDiceScale :
    public CScript
{
    // 무엇이 필요할까요?
    // 반드시 맞는 유도탄 만들어서 공격 속도에 따라 공격하기만 하면 된다.
    // 일단 공격은 DiceScale이 직접적으로 호출하지 않게 설계할 것
    // Dice쪽에서 자신이 들고 있는 CDiceScale에 공격 함수를 호출하면,
    // DiceScale이 새로운 DiceScaleProjectile을 만들어서 target Enemy 쪽에 계속해서 날아가도록 하고,
    // DiceScale이 Target Enemy에게 다가갈 경우 -> 효과를 적용해준다.
            // 주사위 별로 다른 동작을 하는, 갈아끼울 수 있는 효과.
                // 1. 데미지
                // 2. 애니메이션

private:

    // 소속 주사위 
    CDiceScript*                m_pOwnerDiceScript;

    // 공격 우선 순위
    ATTACK_PRIORITY             m_AttackPriority = ATTACK_PRIORITY::FRONT;

    // 추사위 색상(투사체 색상 위해)
    Vec3                        m_vDiceColor;

    // 타겟 enemy
    CEnemyScript*               m_pTargetEnemy;

    // 필드
    CFieldScript*               m_pField;

public:
    // 투사체 만들어서 타겟 오브젝트에게 공격
    void Attack();

    void SetDiceScript(CDiceScript* _OwnerDice) { m_pOwnerDiceScript = _OwnerDice; }
    
    void SetField(CFieldScript* _OwnerField) { m_pField = _OwnerField; }

    void SetDiceColor(Vec3 _DiceColor) { m_vDiceColor = _DiceColor; }

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}
public:
    CLONE(CDiceScale);
    CDiceScale();
    CDiceScale(const CDiceScale& _Origin);
    ~CDiceScale();

};

