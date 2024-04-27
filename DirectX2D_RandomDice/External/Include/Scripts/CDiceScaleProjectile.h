#pragma once
#include <Engine\CScript.h>
#include "ScriptDefine.h"

// 객체를 많이많이 할당할 예정이기 때문에, 최대한 적은 메모리 사용이 필요해보인다.
class CDiceScaleProjectile :
    public CScript
{
private:
    class CFieldScript*         m_pField;       // 타겟 적을 가져오기 위해
    ENEMY_PAIR                  m_pTargetEnemy; // 타겟 적을 저장하고 데미지 콜백 함수를 호출하기 위해
    
    class CDiceAttackScript*    m_DiceAttack; // 이 것만 있으면 되는거 아닐까? 여기서 콜백을 호출하고, 여기서 데미지를 입히는데.

    Vec3                        m_ProjectileColor;

    CGameObject*                m_ProjectileAnimObject;
    bool                        m_IsDeadProjectile;
    float                       m_ProjectileDeadTimer;

public:
    // 투사체를 멈추고, 피격 애니메이션을 재생
    void StopProjectile();

    void SetField(CFieldScript* _pField) { m_pField = _pField; }
    void SetColor(Vec3 _Color) { m_ProjectileColor = _Color; }
    void SetDiceAttackScript(CDiceAttackScript* _DiceAttack) 
    {
        m_DiceAttack = _DiceAttack; 
        GetOwner()->AddComponent((CComponent*)m_DiceAttack);
    }

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

