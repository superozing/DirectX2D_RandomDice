#pragma once
#include <Engine\CScript.h>
#include <Engine\CAnimator2D.h>

class CDiceScaleProjectile :
    public CScript
{
private:

    // 얘는 그냥 계속 TargetEnemy 위치로 날아가기만 하면 된다.
    // 정확히 무엇이 필요하냐면..
    // 1. 데미지 콜백 함수가 필요하겠죠?
    //  왜 콜백을 사용하냐면요...
    // 음.. 얘는 조금 다른 쉐이더를 사용해야 해요.
    // Vec4로 색상 인자를 넣어주어야 해요

    class CFieldScript*         m_pField;
    class CEnemyScript*         m_pTargetEnemy;

    Vec3                        m_ProjectileColor;



public:

    void StopProjectile();
    void SetField(CFieldScript* _pField) { m_pField = _pField; }




public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CDiceScaleProjectile);
    CDiceScaleProjectile();
    CDiceScaleProjectile(const CDiceScaleProjectile& _Origin);
    ~CDiceScaleProjectile();
};

