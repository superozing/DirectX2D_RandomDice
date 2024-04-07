#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"


class CDiceAttackScript :
    public CScript
{
private:
    // ���� �켱 ����
    ATTACK_PRIORITY     m_AttackPriority;

    // ���ݷ°� ���� �ӵ�
    float               m_AttackDamage;
    float               m_AttackSpeed;

public:
    // ���� ȿ��(CALLBACK)

    // 1. ���� AttackDelegate�� ���ؼ� DelegateFunc ȣ��
    //      => (diceAttackScript��ü������->*AttackDelegate)(ENEMY_PAIR ��ü);
    // 2. CallAttackDelegateFunc()�� ����� ȣ��
    EnemyAttackDelegate AttackDelegate;

    // ���� ��������Ʈ �Լ� ȣ��
    void CallAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
    {
        if (AttackDelegate != nullptr)
        {
            (this->*AttackDelegate)(_EnemyPair);
        }
    }

    // 	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
    // ���� ���� ����Ͽ� �Ļ� Ŭ�������� ����
    void SetAttackDelegate(EnemyAttackDelegate _DelegateFunc) { AttackDelegate = _DelegateFunc; }

public:
    void SetAttackPriority(ATTACK_PRIORITY _ATTACK_PRIORITY) { m_AttackPriority = _ATTACK_PRIORITY; }
    void SetAttackDamage(float _AttackDamage) { m_AttackDamage = _AttackDamage; }
    void SetAttackSpeed(float _AttackSpeed) { m_AttackSpeed = _AttackSpeed; }
    
    ATTACK_PRIORITY GetAttackPriority() const { return m_AttackPriority; }
    float GetAttackDamage() const { return m_AttackDamage; }
    float GetAttackSpeed() const { return m_AttackSpeed; }

private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceAttackScript);
    CDiceAttackScript();
    CDiceAttackScript(UINT _ScriptType);
    CDiceAttackScript(const CDiceAttackScript& _Origin);
    ~CDiceAttackScript();
};

