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

static const wstring DicePathArr[(UINT)DICE::END]
{
    L"NONE",

    L"texture\\Dice\\01_fire.png",
    L"texture\\Dice\\02_electric.png",
    L"texture\\Dice\\03_wind.png",
    L"texture\\Dice\\04_poison.png",
    L"texture\\Dice\\05_ice.png",
    L"texture\\Dice\\06_iron.png",
    L"texture\\Dice\\07_broken.png",
    L"texture\\Dice\\08_gamble.png",
    L"texture\\Dice\\09_lock.png",
    L"texture\\Dice\\10_mine.png",
    L"texture\\Dice\\11_light.png",
    L"texture\\Dice\\12_thorn.png",
    L"texture\\Dice\\13_crack.png",
    L"texture\\Dice\\14_critical.png",
    L"texture\\Dice\\15_energy.png",
    L"texture\\Dice\\16_sacrifice.png",
    L"texture\\Dice\\17_arrow.png",
    L"texture\\Dice\\18_mimic.png"
};
class CExternScript :
    public CScript
{
private:
    vector<int>     m_diceDeck;

public:

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:

    void SetDice(UINT _idx, DICE _diceType);

    wstring GetDicePathWString(DICE _diceType)
    {
        return DicePathArr[(UINT)_diceType];
    }



public:
    CLONE_DISABLE(CExternScript);
    CExternScript();
    ~CExternScript();
};

