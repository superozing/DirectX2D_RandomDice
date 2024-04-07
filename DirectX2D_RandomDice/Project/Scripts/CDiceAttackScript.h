#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"


class CDiceAttackScript :
    public CScript
{
private:
    // 공격 우선 순위
    ATTACK_PRIORITY     m_AttackPriority;

    // 공격력과 공격 속도
    float               m_AttackDamage;
    float               m_AttackSpeed;

public:
    // 공격 효과(CALLBACK)

    // 1. 직접 AttackDelegate를 통해서 DelegateFunc 호출
    //      => (diceAttackScript객체포인터->*AttackDelegate)(ENEMY_PAIR 객체);
    // 2. CallAttackDelegateFunc()를 사용해 호출
    EnemyAttackDelegate AttackDelegate;

    // 공격 델리게이트 함수 호출
    void CallAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
    {
        if (AttackDelegate != nullptr)
        {
            (this->*AttackDelegate)(_EnemyPair);
        }
    }

    // 	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
    // 위와 같이 사용하여 파생 클래스에서 세팅
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

