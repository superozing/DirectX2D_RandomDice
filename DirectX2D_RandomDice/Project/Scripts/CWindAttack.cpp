#include "pch.h"
#include "CWindAttack.h"

#include "CEnemyScript.h"

CWindAttack::CWindAttack()
	: CDiceAttackScript(WINDATTACK)
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CWindAttack::WindAttackDelegateFunc);
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CWindAttack::CWindAttack(const CWindAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CWindAttack::WindAttackDelegateFunc);
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CWindAttack::~CWindAttack()
{
}

void CWindAttack::WindAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
{
	_EnemyPair.pEnemyScript->TakeDamage(GetAttackDamage());
}
