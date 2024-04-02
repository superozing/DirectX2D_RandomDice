#include "pch.h"
#include "CDiceScale.h"

CDiceScale::CDiceScale()
	: CScript(SCRIPT_TYPE::DICESCALE)
{
}

CDiceScale::CDiceScale(const CDiceScale& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CDiceScale::~CDiceScale()
{
}

void CDiceScale::Attack()
{
	// 음.. 사실 여기서 새로운 투사체를 만들어도 되는거지만
	// 프리팹을 사용하는게 조금 더 효율적이지 않을까?
	// 일단은 프리팹을 사용하지 말자.

	m_pTargetEnemy;





}

void CDiceScale::ClearDiceScale()
{
	for (auto& it : m_ProjectileList)
		it.StopProjectile();



}
