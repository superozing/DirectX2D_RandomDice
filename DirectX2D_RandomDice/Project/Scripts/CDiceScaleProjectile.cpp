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
	// 피격 효과(넣어주어야 함)
	// 사실 피격 효과는 눈금이 투사체에게 효과를 넣어주는 것이 아니라, 그냥 CDiceScale의 콜백을 호출해주어야 한다.
	// 그러면 눈금의 포인터도 멤버로 들고 있어야겠네요.

	// 피격 애니메이터를 가진 자식 오브젝트 
	// 피격시 효과 정보
	
	// MeshRender Set
	GetOwner()->AddComponent(new CMeshRender);
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_eye.png", L"texture\\Dice\\dice_eye.png"));
}

void CDiceScaleProjectile::tick()
{
	// 이 오브젝트가 Dead 상태일 경우 바로 return
	if (GetOwner()->IsDead())
		return;
	
	// 타겟 오브젝트가 아직 설정되지 않았을 경우
	if (m_pTargetEnemy.pObject == nullptr)
	{
		// 필드로부터 타겟 가져오기
		m_pTargetEnemy = m_pField->GetTargetEnemy(m_DiceAttack->GetAttackPriority());

		// 만약 가져온 타겟 오브젝트가 nullptr -> 해당 공격 우선 순위 적을 찾지 못함.
		if (m_pTargetEnemy.pObject == nullptr)
		{
			// 파괴
			GamePlayStatic::DestroyGameObject(GetOwner());
			return;
		}
	}

	// 만약 타겟이 Dead일 경우
	if (m_pTargetEnemy.pObject->IsDead() || m_pTargetEnemy.pEnemyScript->IsDeadEnemy())
	{
		// 나중에 애니메이션을 추가해야 한다.
		GamePlayStatic::DestroyGameObject(GetOwner());
		return;
	}


	// 적을 향해 날아가기

	// 적을 향하는 방향을 구한다.
	Vec2 vEnemy = Vec2(m_pTargetEnemy.pObject->Transform()->GetWorldPos().x, m_pTargetEnemy.pObject->Transform()->GetWorldPos().y);
	Vec2 vProj = Vec2(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y);

	Vec2 vDir = vEnemy - vProj;

	// 적에게 닿았을 경우.
	if (vDir.Length() < 6.f)
	{
		// 나중에 애니메이션을 추가해야 한다.
		m_pTargetEnemy.pEnemyScript->TakeDamage(m_DiceAttack->GetAttackDamage()); // 나중에 주사위 종류에 따른 데미지를 주어야 한다.
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else 
	{
		// 정규화 - 방향 정보를 가지게
		vDir.Normalize();

		int m_Speed = 1000;

		vProj += vDir * m_Speed * DT;
		Transform()->SetRelativePos(Vec3(vProj.x, vProj.y, Transform()->GetWorldPos().z));
	}

	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor, 1));
}

void CDiceScaleProjectile::StopProjectile()
{
}