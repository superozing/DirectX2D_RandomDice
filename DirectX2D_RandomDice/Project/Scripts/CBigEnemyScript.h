#pragma once
#include "CEnemyScript.h"
class CBigEnemyScript : 
	public CEnemyScript
{
public:

    CLONE(CBigEnemyScript);
    CBigEnemyScript();
    CBigEnemyScript(const CBigEnemyScript& _Origin);
    ~CBigEnemyScript();
};

