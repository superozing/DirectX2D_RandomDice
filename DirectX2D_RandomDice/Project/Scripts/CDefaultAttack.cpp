#include "pch.h"
#include "CDefaultAttack.h"

CDefaultAttack::CDefaultAttack()
	: CDiceAttackScript(DEFAULTATTACK)
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(1.f);
}

CDefaultAttack::CDefaultAttack(const CDefaultAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(1.f);
}

CDefaultAttack::~CDefaultAttack()
{
}
