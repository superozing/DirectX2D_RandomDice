#pragma once
#include "CEnemyScript.h"
class CSpeedEnemyScript :
    public CEnemyScript
{

public:

    CLONE(CSpeedEnemyScript);
    CSpeedEnemyScript();
    CSpeedEnemyScript(const CSpeedEnemyScript& _Origin);
    ~CSpeedEnemyScript();
};

