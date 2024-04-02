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
	// ��.. ��� ���⼭ ���ο� ����ü�� ���� �Ǵ°�����
	// �������� ����ϴ°� ���� �� ȿ�������� ������?
	// �ϴ��� �������� ������� ����.
	
	CGameObject* pProjectile = new CGameObject;

	GamePlayStatic::SpawnGameObject(pProjectile, 7);

	pProjectile->AddComponent(new CTransform);
	
	// ����ü�� ��� ������ ���� �������� ����
	pProjectile->Transform()->SetRelativePos(Transform()->GetWorldPos());
	pProjectile->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

	auto pPScript = new CDiceScaleProjectile;
	pProjectile->AddComponent(pPScript);


	//pPScript->SetField



}

