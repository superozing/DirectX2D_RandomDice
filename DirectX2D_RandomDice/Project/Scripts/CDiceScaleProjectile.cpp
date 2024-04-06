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
	// �ǰ� ȿ��(�־��־�� ��)
	// ��� �ǰ� ȿ���� ������ ����ü���� ȿ���� �־��ִ� ���� �ƴ϶�, �׳� CDiceScale�� �ݹ��� ȣ�����־�� �Ѵ�.
	// �׷��� ������ �����͵� ����� ��� �־�߰ڳ׿�.

	// �ǰ� �ִϸ����͸� ���� �ڽ� ������Ʈ 
	// �ǰݽ� ȿ�� ����
	
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
	// �� ������Ʈ�� Dead ������ ��� �ٷ� return
	if (GetOwner()->IsDead())
		return;
	
	// ������ ���� Projectile�� ���
	if (m_IsDeadProjectile)
	{
		// �ణ�� ������ �ֳ׿�.
		// �ǰ� �ִϸ��̼��� �⺻ �ǰ� �ִϸ��̼� �Ӹ� �ƴ϶�, �ٸ� �ִϸ��̼ǵ� ���� �� �ִµ�
		// �̷��� ��� �����ؾ� �ұ��?

		m_ProjectileDeadTimer += DT;

		// �ִϸ��̼� ����� ������ ��� ������Ʈ ����.
		if (m_ProjectileDeadTimer > 0.36f)
			GamePlayStatic::DestroyGameObject(GetOwner());

		return;
	}

	// Ÿ�� ������Ʈ�� ���� �������� �ʾ��� ���
	if (m_pTargetEnemy.pObject == nullptr)
	{
		// �ʵ�κ��� Ÿ�� ��������
		m_pTargetEnemy = m_pField->GetTargetEnemy(m_DiceAttack->GetAttackPriority());

		// ���� ������ Ÿ�� ������Ʈ�� nullptr -> �ش� ���� �켱 ���� ���� ã�� ����.
		if (m_pTargetEnemy.pObject == nullptr)
		{
			// �ı� �ִϸ��̼� ���
			StopProjectile();

			return;
		}
	}

	// ���� Ÿ���� Dead�� ���
	if (m_pTargetEnemy.pObject->IsDead() || m_pTargetEnemy.pEnemyScript->IsDeadEnemy())
	{
		// �ı� �ִϸ��̼� ���
		StopProjectile();

		return;
	}


	// ���� ���� ���ư���

	// ���� ���ϴ� ������ ���Ѵ�.
	Vec2 vEnemy = Vec2(m_pTargetEnemy.pObject->Transform()->GetWorldPos().x, m_pTargetEnemy.pObject->Transform()->GetWorldPos().y);
	Vec2 vProj = Vec2(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y);

	Vec2 vDir = vEnemy - vProj;

	// ������ ����� ���.
	if (vDir.Length() < 6.f)
	{
		m_pTargetEnemy.pEnemyScript->TakeDamage(m_DiceAttack->GetAttackDamage()); // ���߿� �ֻ��� ������ ���� �������� �־�� �Ѵ�.

		// �ı� �ִϸ��̼� ���
		StopProjectile();
	}
	else 
	{
		// ����ȭ - ���� ������ ������
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
	// ����ü Dead ����
	m_IsDeadProjectile = true;

	// ���� ����
	m_ProjectileAnimObject->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 1);
	m_ProjectileAnimObject->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor, 1));

	// �ı� �ִϸ��̼� ���
	m_ProjectileAnimObject->Animator2D()->Play(L"00_default", false);

	// ����ü�� �Ⱥ��̵��� �ڷ� �о����
	Transform()->SetRelativePos(Vec3(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y, 1100.f));

	m_ProjectileAnimObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -1000.f));
	m_ProjectileAnimObject->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));
}