#pragma once
#include <Engine\CScript.h>

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

    class CDiceScale*       m_pDiceScale;
    class CEnemyScript*     m_pTargetEnemy;

public:

    void SetTargetEnemy(CEnemyScript* _pEnemy)
    {
        m_pTargetEnemy = _pEnemy;
    }
    CEnemyScript* GetTargetEnemy()
    {
        return m_pTargetEnemy;
    }

    // ����ü �ߴ� �Լ�
    void StopProjectile();





public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(CDiceScaleProjectile);
    CDiceScaleProjectile();
    CDiceScaleProjectile(const CDiceScaleProjectile& _Origin);
    ~CDiceScaleProjectile();
};

