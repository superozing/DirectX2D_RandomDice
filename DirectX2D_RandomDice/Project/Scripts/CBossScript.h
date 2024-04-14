#pragma once
#include "CEnemyScript.h"

class CBossScript :
    public CEnemyScript
{

public:
    virtual void BossSkill() override {}

    virtual void SetDeadEnemy() override;

public:
    virtual void begin() override {}
    virtual void tick() override {}

public:
    FSAVELOAD_DISABLE;
    CLONE(CBossScript);
    CBossScript();
    CBossScript(SCRIPT_TYPE _ScriptType);
    CBossScript(const CBossScript& _Origin);
    ~CBossScript();
};

