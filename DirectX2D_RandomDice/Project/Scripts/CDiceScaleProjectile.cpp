#include "pch.h"
#include "CDiceScaleProjectile.h"
#include "CFieldScript.h"

CDiceScaleProjectile::CDiceScaleProjectile()
	: CScript(DICESCALEPROJECTILE)
{
}

CDiceScaleProjectile::CDiceScaleProjectile(const CDiceScaleProjectile& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CDiceScaleProjectile::~CDiceScaleProjectile()
{
}

void CDiceScaleProjectile::begin()
{
	// 1. 외부에서 넣어주는 것
	// 필드
	// 색상
	// 피격 효과(넣어주어야 함)

	// 2. 필요한 것
	// 피격 애니메이터를 가진 자식 오브젝트 
	// 피격시 효과 정보
	

	// 타겟 적
	m_pTargetEnemy = m_pField->GetTargetEnemy(m_AttackPriority);


	GetOwner()->AddComponent(new CMeshRender);
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_eye.png", L"texture\\Dice\\dice_eye.png"));

	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor, 0));
}

void CDiceScaleProjectile::tick()
{
	if (m_pTargetEnemy.pObject == nullptr)
	{
		m_pTargetEnemy = m_pField->GetTargetEnemy(m_AttackPriority);
	}
	if (m_pTargetEnemy.pObject == nullptr)
	{
		return;
	}

	// 해야 할 일
	// 1) 만약 타겟 적이 죽었는가 -> 죽었다면 예외처리
	if (m_pTargetEnemy.pObject->IsDead())
	{
		m_pTargetEnemy = ENEMY_PAIR();

		// 나중에 애니메이션을 추가해야 한다.
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	// 3) 적을 향해 날아가기

	// 몬스터를 향하는 방향을 구한다.
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vDir = m_pTargetEnemy.pObject->Transform()->GetWorldPos();
	
	// 4) 적에게 닿았을 경우.
	if (abs(Vec3::Distance(vPos, vDir)) < 3.f)
	{
		// 나중에 애니메이션을 추가해야 한다.
		// 나중에 주사위 종류에 따른 데미지를 주어야 한다.
		m_pTargetEnemy.pEnemyScript->TakeDamage(50);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else
	{
		vDir.x -= vPos.x;
		vDir.y -= vPos.y;


		vDir.Normalize();

		vPos.x += vDir.x * 500 * DT;
		vPos.y += vDir.y * 500 * DT;

		Transform()->SetRelativePos(vPos);
	}
}

void CDiceScaleProjectile::StopProjectile()
{
}