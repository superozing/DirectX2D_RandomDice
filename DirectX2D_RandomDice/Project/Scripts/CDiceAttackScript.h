#pragma once
#include <Engine/CScript.h>

enum class ATTACK_PRIORITY // �ֻ����� ���� �켱 ����
{
    BUFF,		 // ���� �ֻ��� - ������ ���� ����
    FRONT,		 // �� �� �� �켱 ����
    HIGH_HEALTH, // ���� ü���� �� �켱 ����
    RANDOM,      // ������ �� ����
    RANGE_ATTACK,// ������ ���� ����
    END,
};

// �ֻ��� ���� ���� ��ũ��Ʈ
class CDiceAttackScript :
    public CScript
{
private:
    // ���� �켱 ����
    ATTACK_PRIORITY     m_AttackPriority;

    // ���ݷ°� ���� �ӵ�
    float               m_AttackDamage;
    float               m_AttackSpeed;

    // ���� ȿ��(CALLBACK)
    // void(*pFunc)() m_,,,��¼����¼��

    // �ǰ� �ִϸ��̼�
    // ���߿� ����.

public:
    void SetAttackPriority(ATTACK_PRIORITY _ATTACK_PRIORITY) { m_AttackPriority = _ATTACK_PRIORITY; }
    void SetAttackDamage(float _AttackDamage) { m_AttackDamage = _AttackDamage; }
    void SetAttackSpeed(float _AttackSpeed) { m_AttackSpeed = _AttackSpeed; }
    
    ATTACK_PRIORITY SetAttackPriority() const { return m_AttackPriority; }
    float SetAttackDamage() const { return m_AttackDamage; }
    float SetAttackSpeed() const { return m_AttackSpeed; }

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

