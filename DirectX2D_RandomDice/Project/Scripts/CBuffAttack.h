#pragma once
#include "CDiceAttackScript.h"
class CBuffAttack :
    public CDiceAttackScript
{
private:


public:
    CLONE(CBuffAttack);
    CBuffAttack();
    CBuffAttack(const CBuffAttack& _Origin);
    ~CBuffAttack();
};

