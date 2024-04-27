#pragma once
#include <Engine\CScript.h>
#include "ScriptDefine.h"

// 주사위 눈금 스크립트
class CDiceScale :
    public CScript
{
                // 애니메이션 필요
private:

    // 소속 주사위 
    class CDiceScript*                m_pOwnerDiceScript;

    class CDiceAttackScript*          m_pDiceAttack;

    // 공격 우선 순위
    ATTACK_PRIORITY             m_AttackPriority = ATTACK_PRIORITY::FRONT;

    // 추사위 색상(투사체 색상 위해)
    Vec3                        m_vDiceColor;

    // 타겟 enemy
    class CEnemyScript*               m_pTargetEnemy;

    // 필드
    class CFieldScript*               m_pField;


    Vec3                        m_SrcScale = Vec3(10.f, 10.f, 1.f);
    float                       m_ScaleSize = 1.f;


public:
    // 투사체 만들어서 타겟 오브젝트에게 공격
    void Attack();

    void SetDiceScript(CDiceScript* _OwnerDice) { m_pOwnerDiceScript = _OwnerDice; }
    void SetDiceAttackScript(CDiceAttackScript* _DiceAttack) { m_pDiceAttack = _DiceAttack; }
    
    void SetField(CFieldScript* _OwnerField) { m_pField = _OwnerField; }

    void SetDiceColor(Vec3 _DiceColor) { m_vDiceColor = _DiceColor; }
    void SetSrcScale(Vec3 _SrcScale) { m_SrcScale = _SrcScale; }


public: // 전역
    static Ptr<CPrefab> m_DiceScaleProjectilePrefab;
    static CGameObject* GetDiceScaleProjectile();


public:
    virtual void tick() override;

public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}
public:
    CLONE(CDiceScale);
    CDiceScale();
    CDiceScale(const CDiceScale& _Origin);
    ~CDiceScale();

};

