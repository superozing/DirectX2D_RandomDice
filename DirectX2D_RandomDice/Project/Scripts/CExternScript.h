#pragma once
#include <Engine/CScript.h>

enum class DICE
{
    NONE,

    FIRE,
    ELECTRIC,
    WIND,
    POISON,
    ICE,

    IRON,
    BROKEN,
    GAMBLE,
    LOCK,
    MINE,

    LIGHT,
    THORN,
    CRACK,
    CRITICAL,
    ENERGY,

    SACRIFICE,
    ARROW,
    MIMIC,

    END,
};



enum class DICE_BUFF
{
    NONE,
    CRITICAL,
    ATTACK_SPEED,
};


struct DICE_INFO
{
    // �Ƴ�. �̷� ������ �ϸ� �ȵȴ�.
    // �ʹ� ������ ��������. ���� �̷� ������ �������� �ʾ�������.
    // ���� ��ũ��Ʈ, ���� ��ũ��Ʈ, ������ �� ��ũ��Ʈ �̷��� �� ���� ������.
    // �׸��� �Ļ���Ű�� ���� ���ƺ��δ�.
    class CDiceAttackScript*    pAttack;
    class CDiceBuffScript*      pBuff;
    class CDiceMergeScript*     pMerge;
};





class CExternScript :
    public CScript
{
private:
    vector<int>     m_diceDeck;

public:
    vector<wstring>     DicePath;
    vector<DICE_INFO>   DiceInfo;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:

    void SetDice(UINT _idx, DICE _diceType);

    wstring GetDicePathWString(DICE _diceType)
    {
        return DicePath[(UINT)_diceType];
    }



public:
    CLONE_DISABLE(CExternScript);
    CExternScript();
    ~CExternScript();
};

