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
	// 음.. 사실 여기서 새로운 투사체를 만들어도 되는거지만
	// 프리팹을 사용하는게 조금 더 효율적이지 않을까?
	// 일단은 프리팹을 사용하지 말자.
	
	CGameObject* pProjectile = new CGameObject;
	pProjectile->SetName(L"Projectile");
	GamePlayStatic::SpawnGameObject(pProjectile, 7);

	pProjectile->AddComponent(new CTransform);
	
	// 투사체의 출발 지점을 현재 눈금으로 지정
	pProjectile->Transform()->SetRelativePos(Transform()->GetWorldPos());
	pProjectile->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));

	auto pPScript = new CDiceScaleProjectile;
	pProjectile->AddComponent(pPScript);

	// 필드 설정해주기
	pPScript->SetField(m_pField);
	pPScript->SetColor(m_vDiceColor);
	pPScript->SetAttackPriority(m_AttackPriority);

}

