#pragma once
#include <Engine\CScript.h>
#include <Engine\CAnimator2D.h>
#include "CDiceAttackScript.h"
#include "CFieldScript.h"

class CDiceScaleProjectile :
    public CScript
{
private:

    // 얘는 그냥 계속 TargetEnemy 위치로 날아가기만 하면 된다.
    // 정확히 무엇이 필요하냐면..
    // 1. 데미지 콜백 함수가 필요하겠죠?
    //  왜 콜백을 사용하냐면요...
    // 음.. 얘는 조금 다른 쉐이더를 사용해야 해요.

    class CFieldScript*         m_pField;
    
    ATTACK_PRIORITY             m_AttackPriority;
    ENEMY_PAIR                  m_pTargetEnemy;
    


    Vec3                        m_ProjectileColor;



public:
    void StopProjectile();
    void SetField(CFieldScript* _pField) { m_pField = _pField; }
    void SetColor(Vec3 _Color) { m_ProjectileColor = _Color; }
    void SetAttackPriority(ATTACK_PRIORITY _AttackPriority) { m_AttackPriority = _AttackPriority; }






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

