#pragma once
#include "CDiceAttackScript.h"
class CElectricAttack :
    public CDiceAttackScript
{
private:
    class CFieldScript* m_Field;

public:
    void ElectricAttackDelegateFunc(ENEMY_PAIR _EnemyPair);

public:
    CLONE(CElectricAttack);
    CElectricAttack();
    CElectricAttack(const CElectricAttack& _Origin);
    ~CElectricAttack();
};

