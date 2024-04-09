#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"
class CDiceMergeScript :
    public CScript
{
private:
    CDiceScript*        m_OwnerDice;

    DiceMergeDelegate   MergeDelegateFunc;
    IsMergeAbleDelegate MergeAbleDelegateFunc;

public:

    // 	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
    void SetMergeDelegate(DiceMergeDelegate _DelegateFunc) { MergeDelegateFunc = _DelegateFunc; }
    
    void MergeDice(CDiceScript* _FocusDice) // �ռ� ��������Ʈ �Լ� ȣ��
    {
        if (GetOwnerDice() && MergeDelegateFunc) // �� �� nullptr�� �ƴ� ���
            (this->*MergeDelegateFunc)(_FocusDice);
    }

    // 	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
    void SetIsMergeAbleDelegate(IsMergeAbleDelegate _DelegateFunc) { MergeAbleDelegateFunc = _DelegateFunc; }

    bool IsMergeAble(CDiceScript* _FocusDice)
    {
        if (GetOwnerDice() && MergeAbleDelegateFunc)
            return (this->*MergeAbleDelegateFunc)(_FocusDice);

        return false;
    }


    void SetOwnerDice(CDiceScript* _Dice) { m_OwnerDice = _Dice; }

protected:
    CDiceScript* GetOwnerDice() { return m_OwnerDice; }

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceMergeScript);
    CDiceMergeScript();
    CDiceMergeScript(UINT _ScriptType);
    CDiceMergeScript(const CDiceMergeScript& _Origin);
    ~CDiceMergeScript();
};

