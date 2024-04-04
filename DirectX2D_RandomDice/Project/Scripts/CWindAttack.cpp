#include "pch.h"
#include "CWindAttack.h"

CWindAttack::CWindAttack()
	: CDiceAttackScript(WINDATTACK)
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(0.3f);
}

CWindAttack::CWindAttack(const CWindAttack& _Origin)
	: CDiceAttackScript(_Origin.GetScriptType())
{
	SetAttackDamage(50.f);
	SetAttackPriority(ATTACK_PRIORITY::FRONT);
	SetAttackSpeed(0.3f);
}

CWindAttack::~CWindAttack()
{
}
