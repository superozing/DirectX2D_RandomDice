#pragma once
#include <Engine\CScript.h>
#include "ScriptDefine.h"

// �ֻ��� ���� ��ũ��Ʈ
class CDiceScale :
    public CScript
{
                // �ִϸ��̼� �ʿ�
private:

    // �Ҽ� �ֻ��� 
    class CDiceScript*                m_pOwnerDiceScript;

    class CDiceAttackScript*          m_pDiceAttack;

    // ���� �켱 ����
    ATTACK_PRIORITY             m_AttackPriority = ATTACK_PRIORITY::FRONT;

    // �߻��� ����(����ü ���� ����)
    Vec3                        m_vDiceColor;

    // Ÿ�� enemy
    class CEnemyScript*               m_pTargetEnemy;

    // �ʵ�
    class CFieldScript*               m_pField;


    Vec3                        m_SrcScale = Vec3(10.f, 10.f, 1.f);
    float                       m_ScaleSize = 1.f;


public:
    // ����ü ���� Ÿ�� ������Ʈ���� ����
    void Attack();

    void SetDiceScript(CDiceScript* _OwnerDice) { m_pOwnerDiceScript = _OwnerDice; }
    void SetDiceAttackScript(CDiceAttackScript* _DiceAttack) { m_pDiceAttack = _DiceAttack; }
    
    void SetField(CFieldScript* _OwnerField) { m_pField = _OwnerField; }

    void SetDiceColor(Vec3 _DiceColor) { m_vDiceColor = _DiceColor; }
    void SetSrcScale(Vec3 _SrcScale) { m_SrcScale = _SrcScale; }


public: // ����
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

