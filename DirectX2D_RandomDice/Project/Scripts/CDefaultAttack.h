#pragma once
#include "CDiceAttackScript.h"
class CDefaultAttack :
    public CDiceAttackScript
{
public:
    void DefaultAttackDelegateFunc(ENEMY_PAIR _EnemyPair);

public:
    CLONE(CDefaultAttack);
    CDefaultAttack();
    CDefaultAttack(const CDefaultAttack& _Origin);
    ~CDefaultAttack();
};

