#include "pch.h"
#include "CDiceScale.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CDiceScaleProjectile.h"
#include "CDiceAttackScript.h"
#include "CFieldScript.h"

Ptr<CPrefab> CDiceScale::m_DiceScaleProjectilePrefab = nullptr;

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
	//m_pDiceAttack->SetAttackDelegate(nullptr);
}

void CDiceScale::Attack()
{
	if (m_pField->GetTargetEnemy(m_AttackPriority).pObject == nullptr || m_pDiceAttack == nullptr)
		return;
	
	CGameObject* pObj = CDiceScale::GetDiceScaleProjectile();

	// 프리팹 객체로부터 투사체 스크립트 가져오기
	CDiceScaleProjectile* pProjectile = pObj->GetScript<CDiceScaleProjectile>();

	// Transform set
	pObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));
	pObj->Transform()->SetRelativePos(Transform()->GetWorldPos());
	pObj->Transform()->finaltick();

	// 투사체 정보 세팅
	pProjectile->SetField(m_pField);
	pProjectile->SetColor(m_vDiceColor);

	// 객체에게 begin() 호출
	pObj->begin();

	// 레벨에 스폰
	GamePlayStatic::SpawnGameObject(pObj, 9);


	pProjectile->SetDiceAttackScript(m_pDiceAttack->Clone());

	
	// 공격 효과
	m_ScaleSize = 1.5f;
}


void CDiceScale::tick()
{
	if (m_ScaleSize > 1.f)
	{
		m_ScaleSize -= DT * 3;

		if (m_ScaleSize < 1.f)
			m_ScaleSize = 1.f;
	}

	if (Transform() != nullptr)
	{
		Transform()->SetRelativeScale(m_SrcScale * m_ScaleSize);
	}
}


CGameObject* CDiceScale::GetDiceScaleProjectile()
{
	// 첫 실행 시 프리팹 로드
	if (m_DiceScaleProjectilePrefab == nullptr)
	{
		m_DiceScaleProjectilePrefab = CAssetMgr::GetInst()->Load<CPrefab>(
			L"prefab\\DiceScaleProjectile.pref", L"prefab\\DiceScaleProjectile.pref");
	}

	return m_DiceScaleProjectilePrefab->Instantiate();
}