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
    ATTACK_PRIORITY             m_AttackPriority;

    // �� ������ �� ����ü ����Ʈ. ���� Ÿ�� ������Ʈ�� �ٲ���ٸ� ���ư��� ��ƼŬ�� ���� �����־�� �Ѵ�.
    // �׷��� ����Ʈ�� ��Ƴ���
    list<CDiceScaleProjectile>  m_ProjectileList;
    

    // �߻��� ����(����ü ���� ����)
    Vec3                        m_vDiceColor;

    // Ÿ�� enemy
    CEnemyScript*               m_pTargetEnemy;

    // �ʵ�
    CFieldScript*               m_pFieldEnemy;

public:
    // ����ü ���� Ÿ�� ������Ʈ���� ����
    void Attack();

    void SetDiceScript(CDiceScript* _OwnerDice) { m_pOwnerDiceScript = _OwnerDice; }



public:
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}
public:
    CLONE(CDiceScale);
    CDiceScale();
    CDiceScale(const CDiceScale& _Origin);
    ~CDiceScale();

};

