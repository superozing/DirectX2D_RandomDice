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
    // 아냐. 이런 식으로 하면 안된다.
    // 너무 종류가 많아진다. 절대 이런 식으로 구현하지 않았을꺼야.
    // 공격 스크립트, 버프 스크립트, 합쳐질 때 스크립트 이렇게 세 개를 만들자.
    // 그리고 파생시키는 것이 좋아보인다.
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

