#pragma once
#include "CDiceAttackScript.h"
class CWindAttack :
    public CDiceAttackScript
{
private:

public:
    void WindAttackDelegateFunc(ENEMY_PAIR _EnemyPair);

public:
    CLONE(CWindAttack);
    CWindAttack();
    CWindAttack(const CWindAttack& _Origin);
    ~CWindAttack();
};

