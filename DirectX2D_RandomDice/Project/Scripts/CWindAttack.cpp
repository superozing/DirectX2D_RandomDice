#include "pch.h"
#include "CWindAttack.h"

#include "CEnemyScript.h"

void CWindAttack::WindAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
{
	_EnemyPair.pEnemyScript->TakeDamage(GetAttackDamage());
}

CWindAttack::CWindAttack()
	: CDiceAttackScript(WINDATTACK)
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CWindAttack::CWindAttack(const CWindAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CWindAttack::~CWindAttack()
{
}
