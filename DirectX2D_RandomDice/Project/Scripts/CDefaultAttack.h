#pragma once
#include "CDiceAttackScript.h"
class CDefaultAttack :
    public CDiceAttackScript
{
private:


public:
    CLONE(CDefaultAttack);
    CDefaultAttack();
    CDefaultAttack(const CDefaultAttack& _Origin);
    ~CDefaultAttack();
};

