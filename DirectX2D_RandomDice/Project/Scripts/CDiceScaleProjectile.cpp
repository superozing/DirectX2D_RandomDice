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


	GetOwner()->AddComponent(new CMeshRender);
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\dice_eye.png", L"texture\\Dice\\dice_eye.png"));

	Transform()->SetRelativePos(m_pDiceScript->Transform()->GetRelativePos());

}

void CDiceScaleProjectile::tick()
{
	// �� ������Ʈ�� Dead ������ ��� �ٷ� return
	if (GetOwner()->IsDead())
		return;
	
	// Ÿ�� ������Ʈ�� ���� �������� �ʾ��� ���
	if (m_pTargetEnemy.pObject == nullptr)
	{
		// �ʵ�κ��� Ÿ�� ��������
		m_pTargetEnemy = m_pField->GetTargetEnemy(m_AttackPriority);

		// ���� ������ Ÿ�� ������Ʈ�� nullptr -> �ش� ���� �켱 ���� ���� ã�� ����.
		if (m_pTargetEnemy.pObject == nullptr)
		{
			// �ı�
			GamePlayStatic::DestroyGameObject(GetOwner());
			return;
		}
	}

	// ���� Ÿ���� Dead�� ���
	if (m_pTargetEnemy.pObject->IsDead())
	{
		// ���߿� �ִϸ��̼��� �߰��ؾ� �Ѵ�.
		GamePlayStatic::DestroyGameObject(GetOwner());
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
		// ���߿� �ִϸ��̼��� �߰��ؾ� �Ѵ�.
		// ���߿� �ֻ��� ������ ���� �������� �־�� �Ѵ�.
		m_pTargetEnemy.pEnemyScript->TakeDamage(50);
		GamePlayStatic::DestroyGameObject(GetOwner());
	}
	else 
	{
		// ����ȭ - ���� ������ ������
		vDir.Normalize();

		vProj += vDir * /*m_Speed*/1000 * DT;
		Transform()->SetRelativePos(Vec3(vProj.x, vProj.y, Transform()->GetWorldPos().z));
	}


	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_ProjectileColor / 3, 0));
}

void CDiceScaleProjectile::StopProjectile()
{
}