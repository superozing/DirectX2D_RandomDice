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
    // ��.. � ������ �ʿ��ұ��??


    /// ���ݿ� �ʿ��� ����
    // ���� ��ǥ
    ATTACK_PRIORITY     m_AttackPriority; // ���⼭ ������ ���� �켱 ������ ����, ���� ����Ʈ���� �̾Ƴ��� ���Ϳ��� Attack�� ȣ�����ֱ⸸ �ϸ� �ȴ�.

    // ���ݷ°� ���� �ӵ�
    float               m_Damage;
    float               m_AttackSpeed;



    /// ����
    // ���� ����
    UINT                m_DiceScale;

    // ���� ������ �����ų ������ �ε���
    UINT                m_CurAttackDiceScaleIdx;

    // ���� ����
    // ��... ���� ��ƼŬ�� �� ���� �Դ�.
    // ���ƾƾƾ�...




public:


private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceAttackScript);
    CDiceAttackScript();
    CDiceAttackScript(const CDiceAttackScript& _Origin);
    ~CDiceAttackScript();
};

