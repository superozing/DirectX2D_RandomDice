#pragma once
#include <Engine/CScript.h>
#include "ScriptDefine.h"

class CExitBtnUI :
    public CScript
{

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    FSAVELOAD_DISABLE;
    CLONE(CExitBtnUI);
    CExitBtnUI();
    CExitBtnUI(const CExitBtnUI& _Origin);
    ~CExitBtnUI();
};

