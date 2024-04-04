#include "pch.h"
#include "CDiceScale.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CDiceScaleProjectile.h"

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
	if (m_pField->GetTargetEnemy(m_AttackPriority).pObject == nullptr || m_pDiceAttack == nullptr)
		return;
	// 음.. 사실 여기서 새로운 투사체를 만들어도 되는거지만
	// 프리팹을 사용하는게 조금 더 효율적이지 않을까?
	// 일단은 프리팹을 사용하지 말자.
	
	CGameObject* pProjectile = new CGameObject;
	pProjectile->SetName(L"Projectile");

	pProjectile->AddComponent(new CTransform);
	
	pProjectile->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));
	pProjectile->Transform()->SetRelativePos(Transform()->GetWorldPos());
	pProjectile->Transform()->finaltick();

	auto pPScript = new CDiceScaleProjectile;
	pProjectile->AddComponent(pPScript);

	// 필드 설정해주기
	pPScript->SetField(m_pField);
	pPScript->SetColor(m_vDiceColor);
	pPScript->SetDiceAttackScript(m_pDiceAttack);
	pPScript->SetDiceScript(m_pOwnerDiceScript);

	pProjectile->begin();
	GamePlayStatic::SpawnGameObject(pProjectile, 7);
	
}

