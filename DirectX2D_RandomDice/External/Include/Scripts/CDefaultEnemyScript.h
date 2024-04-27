#pragma once
#include "CEnemyScript.h"

class CDefaultEnemyScript :
    public CEnemyScript
{


public:

    CLONE(CDefaultEnemyScript);
    CDefaultEnemyScript();
    CDefaultEnemyScript(const CDefaultEnemyScript& _Origin);
    ~CDefaultEnemyScript();
};

