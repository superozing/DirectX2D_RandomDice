#pragma once
#include <Engine\CScript.h>
#include <Engine\CAnimator2D.h>
#include "CDiceAttackScript.h"
#include "CFieldScript.h"
#include "CDiceScale.h"

class CDiceScaleProjectile :
    public CScript
{
private:
    // ***********************데미지 콜백 함수 동작이 필요*********************************************

    class CFieldScript*         m_pField;
    
    ENEMY_PAIR                  m_pTargetEnemy;
    class CDiceScript*          m_pDiceScript;
    CDiceAttackScript*          m_DiceAttack;

    Vec3                        m_ProjectileColor;

    CGameObject*                m_ProjectileAnimObject;
    bool                        m_IsDeadProjectile;
    float                       m_ProjectileDeadTimer;

public:
    void StopProjectile();
    void SetField(CFieldScript* _pField) { m_pField = _pField; }
    void SetColor(Vec3 _Color) { m_ProjectileColor = _Color; }
    void SetDiceAttackScript(CDiceAttackScript* _DiceAttack) { m_DiceAttack = _DiceAttack; }
    void SetDiceScript(CDiceScript* _Dice) { m_pDiceScript = _Dice; }






public:

    virtual void begin() override;
    virtual void tick() override;



    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CDiceScaleProjectile);
    CDiceScaleProjectile();
    CDiceScaleProjectile(const CDiceScaleProjectile& _Origin);
    ~CDiceScaleProjectile();
};

