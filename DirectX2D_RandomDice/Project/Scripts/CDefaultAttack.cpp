#include "pch.h"
#include "CDefaultAttack.h"
#include "CEnemyScript.h"

void CDefaultAttack::DefaultAttackDelegateFunc(ENEMY_PAIR _EnemyPair)
{
	_EnemyPair.pEnemyScript->TakeDamage(70.f);
}

CDefaultAttack::CDefaultAttack()
	: CDiceAttackScript(DEFAULTATTACK)
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
	SetAttackDamage(70.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(1.f);
}

CDefaultAttack::CDefaultAttack(const CDefaultAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	CDiceAttackScript::SetAttackDelegate((EnemyAttackDelegate)&CDefaultAttack::DefaultAttackDelegateFunc);
	SetAttackDamage(70.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(1.f);
}

CDefaultAttack::~CDefaultAttack()
{
}
