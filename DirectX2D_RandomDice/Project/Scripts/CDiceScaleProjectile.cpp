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
	// 1. �ܺο��� �־��ִ� ��
	// �ʵ�
	// ����
	// �ǰ� ȿ��(�־��־�� ��)

	// 2. �ʿ��� ��
	// �ǰ� �ִϸ����͸� ���� �ڽ� ������Ʈ 
	// �ǰݽ� ȿ�� ����
	

	// Ÿ�� ��
	m_pTargetEnemy = m_pField->GetTargetEnemy(m_AttackPriority);
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_eye.png", L"texture\\Dice\\dice_eye.png"));

	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor, 0));
}

void CDiceScaleProjectile::tick()
{
	// �ؾ� �� ��
	// 1) ���� Ÿ�� ���� �׾��°� -> �׾��ٸ� ����ó��
	if (m_pTargetEnemy.pObject->IsDead())
	{
		// ���߿� �ִϸ��̼��� �߰��ؾ� �Ѵ�.
		GamePlayStatic::DestroyGameObject(GetOwner());
	}

	// 3) ���� ���� ���ư���

	// ���͸� ���ϴ� ������ ���Ѵ�.
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vDir = m_pTargetEnemy.pObject->Transform()->GetWorldPos();
	
	// 4) ������ ����� ���.
	if (abs(Vec3::Distance(vPos, vDir)) < 3.f)
	{
		// ���߿� �ִϸ��̼��� �߰��ؾ� �Ѵ�.
		// ���߿� �ֻ��� ������ ���� �������� �־�� �Ѵ�.
		m_pTargetEnemy.pEnemyScript->TakeDamage(50);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else
	{
		vDir.x -= vPos.x;
		vDir.y -= vPos.y;


		vDir.Normalize();

		vPos.x += vDir.x * 100 * DT;
		vPos.y += vDir.y * 100 * DT;

		Transform()->SetRelativePos(vPos);
	}
}

void CDiceScaleProjectile::StopProjectile()
{
}