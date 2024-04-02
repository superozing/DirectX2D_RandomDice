#pragma once
#include <Engine\CScript.h>
#include <Engine\CAnimator2D.h>

class CDiceScaleProjectile :
    public CScript
{
private:

    // ��� �׳� ��� TargetEnemy ��ġ�� ���ư��⸸ �ϸ� �ȴ�.
    // ��Ȯ�� ������ �ʿ��ϳĸ�..
    // 1. ������ �ݹ� �Լ��� �ʿ��ϰ���?
    //  �� �ݹ��� ����ϳĸ��...
    // ��.. ��� ���� �ٸ� ���̴��� ����ؾ� �ؿ�.
    // Vec4�� ���� ���ڸ� �־��־�� �ؿ�

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

