#pragma once
#include "CDiceAttackScript.h"
class CWindAttack :
    public CDiceAttackScript
{
private:


public:
    CLONE(CWindAttack);
    CWindAttack();
    CWindAttack(const CWindAttack& _Origin);
    ~CWindAttack();
};

