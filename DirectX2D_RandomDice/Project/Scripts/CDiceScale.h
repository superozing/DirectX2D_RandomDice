#pragma once
#include <Engine\CScript.h>
#include "CDiceScaleProjectile.h"
#include "CEnemyScript.h"
#include "CDiceScript.h"

// �ֻ��� ���� ��ũ��Ʈ
class CDiceScale :
    public CScript
{
    // ������ �ʿ��ұ��?
    // �ݵ�� �´� ����ź ���� ���� �ӵ��� ���� �����ϱ⸸ �ϸ� �ȴ�.
    // �ϴ� ������ DiceScale�� ���������� ȣ������ �ʰ� ������ ��
    // Dice�ʿ��� �ڽ��� ��� �ִ� CDiceScale�� ���� �Լ��� ȣ���ϸ�,
    // DiceScale�� ���ο� DiceScaleProjectile�� ���� target Enemy �ʿ� ����ؼ� ���ư����� �ϰ�,
    // DiceScale�� Target Enemy���� �ٰ��� ��� -> ȿ���� �������ش�.
            // �ֻ��� ���� �ٸ� ������ �ϴ�, ���Ƴ��� �� �ִ� ȿ��.
                // 1. ������
                // 2. �ִϸ��̼�

private:

    // �Ҽ� �ֻ��� 
    CDiceScript*                m_pOwnerDiceScript;

    // ���� �켱 ����
    ATTACK_PRIORITY             m_AttackPriority = ATTACK_PRIORITY::FRONT;

    // �߻��� ����(����ü ���� ����)
    Vec3                        m_vDiceColor;

    // Ÿ�� enemy
    CEnemyScript*               m_pTargetEnemy;

    // �ʵ�
    CFieldScript*               m_pField;

public:
    // ����ü ���� Ÿ�� ������Ʈ���� ����
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

