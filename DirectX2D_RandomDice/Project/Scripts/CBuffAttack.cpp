#include "pch.h"
#include "CBuffAttack.h"

CBuffAttack::CBuffAttack()
	: CDiceAttackScript(BUFFATTACK)
{
	SetAttackDamage(10.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CBuffAttack::CBuffAttack(const CBuffAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	SetAttackDamage(10.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(3.f);
}

CBuffAttack::~CBuffAttack()
{
}
