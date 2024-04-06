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

#define OBJECT GetOwner()

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

	m_ProjectileAnimObject = CDiceScript::GetDefaultProjectileAnimObj();

	OBJECT->AddChild(m_ProjectileAnimObject);
}

void CDiceScaleProjectile::tick()
{
	// 이 오브젝트가 Dead 상태일 경우 바로 return
	if (GetOwner()->IsDead())
		return;
	
	// 동작을 멈춘 Projectile의 경우
	if (m_IsDeadProjectile)
	{
		// 약간의 문제가 있네요.
		// 피격 애니메이션이 기본 피격 애니메이션 뿐만 아니라, 다른 애니메이션도 있을 수 있는데
		// 이런건 어떻게 관리해야 할까요?

		m_ProjectileDeadTimer += DT;

		// 애니메이션 재생이 끝났을 경우 오브젝트 삭제.
		if (m_ProjectileDeadTimer > 0.36f)
			GamePlayStatic::DestroyGameObject(GetOwner());

		return;
	}

	// 타겟 오브젝트가 아직 설정되지 않았을 경우
	if (m_pTargetEnemy.pObject == nullptr)
	{
		// 필드로부터 타겟 가져오기
		m_pTargetEnemy = m_pField->GetTargetEnemy(m_DiceAttack->GetAttackPriority());

		// 만약 가져온 타겟 오브젝트가 nullptr -> 해당 공격 우선 순위 적을 찾지 못함.
		if (m_pTargetEnemy.pObject == nullptr)
		{
			// 파괴 애니메이션 재생
			StopProjectile();

			return;
		}
	}

	// 만약 타겟이 Dead일 경우
	if (m_pTargetEnemy.pObject->IsDead() || m_pTargetEnemy.pEnemyScript->IsDeadEnemy())
	{
		// 파괴 애니메이션 재생
		StopProjectile();

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
		m_pTargetEnemy.pEnemyScript->TakeDamage(m_DiceAttack->GetAttackDamage()); // 나중에 주사위 종류에 따른 데미지를 주어야 한다.

		// 파괴 애니메이션 재생
		StopProjectile();
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
	// 투사체 Dead 설정
	m_IsDeadProjectile = true;

	// 색상 설정
	m_ProjectileAnimObject->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 1);
	m_ProjectileAnimObject->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor, 1));

	// 파괴 애니메이션 재생
	m_ProjectileAnimObject->Animator2D()->Play(L"00_default", false);

	// 투사체를 안보이도록 뒤로 밀어놓기
	Transform()->SetRelativePos(Vec3(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y, 1100.f));

	m_ProjectileAnimObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1000.f));
	m_ProjectileAnimObject->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));
}