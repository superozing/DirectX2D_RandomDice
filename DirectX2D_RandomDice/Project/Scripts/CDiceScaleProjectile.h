#pragma once
#include <Engine\CScript.h>
#include "ScriptDefine.h"

// ��ü�� ���̸��� �Ҵ��� �����̱� ������, �ִ��� ���� �޸� ����� �ʿ��غ��δ�.
class CDiceScaleProjectile :
    public CScript
{
private:
    class CFieldScript*         m_pField;       // Ÿ�� ���� �������� ����
    ENEMY_PAIR                  m_pTargetEnemy; // Ÿ�� ���� �����ϰ� ������ �ݹ� �Լ��� ȣ���ϱ� ����
    
    class CDiceAttackScript*    m_DiceAttack; // �� �͸� ������ �Ǵ°� �ƴұ�? ���⼭ �ݹ��� ȣ���ϰ�, ���⼭ �������� �����µ�.

    Vec3                        m_ProjectileColor;

    CGameObject*                m_ProjectileAnimObject;
    bool                        m_IsDeadProjectile;
    float                       m_ProjectileDeadTimer;

public:
    // ����ü�� ���߰�, �ǰ� �ִϸ��̼��� ���
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

