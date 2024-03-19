#pragma once
#include <Engine/CScript.h>







class CExternScript :
    public CScript
{
private:
    vector<int>     m_diceDeck;

public:
    //vector<wstring>     DicePath;
    //vector<DICE_INFO>   DiceInfo;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:

    //void SetDice(UINT _idx, DICE _diceType);

    //wstring GetDicePathWString(DICE _diceType)
    //{
    //    return DicePath[(UINT)_diceType];
    //}



public:
    CLONE_DISABLE(CExternScript);
    CExternScript();
    ~CExternScript();
};

