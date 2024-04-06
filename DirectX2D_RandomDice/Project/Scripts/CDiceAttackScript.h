#pragma once
#include <Engine/CScript.h>

typedef  void(CEntity::* EnemyAttackDelegate)(ENEMY_PAIR);

enum class ATTACK_PRIORITY // 주사위의 공격 우선 순위
{
    BUFF,		 // 버프 주사위 - 공격을 하지 않음
    FRONT,		 // 앞 쪽 적 우선 공격
    HIGH_HEALTH, // 높은 체력의 적 우선 공격
    RANDOM,      // 무작위 적 공격
    RANGE_ATTACK,// 무작위 범위 공격
    END,
};

// 주사위 공격 정보 스크립트
class CDiceAttackScript :
    public CScript
{
private:
    // 공격 우선 순위
    ATTACK_PRIORITY     m_AttackPriority;

    // 공격력과 공격 속도
    float               m_AttackDamage;
    float               m_AttackSpeed;

    // 공격 효과(CALLBACK)
    EnemyAttackDelegate m_AttackDelegate;


    // 피격 애니메이션
    // 나중에 넣자.

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

