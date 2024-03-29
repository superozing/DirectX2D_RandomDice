#include "pch.h"
#include "CPlayerHP.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CFieldScript.h"

CPlayerHP::CPlayerHP()
	: CScript(PLAYERHP)
{
}

CPlayerHP::CPlayerHP(const CPlayerHP& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CPlayerHP::~CPlayerHP()
{
}

#define OBJECT GetOwner()

void CPlayerHP::begin()
{
	// �����κ��� �ʵ� ��ũ��Ʈ ��������
	m_pField = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"FieldObject")->GetScript<CFieldScript>();

	assert(OBJECT);

	// Transform Set
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	Vec3 vPos = m_pField->OBJECT->Transform()->GetRelativePos();

	OBJECT->Transform()->SetRelativePos(Vec3(vPos.x - 125.f, vPos.y - 75.f, vPos.z - 100.f));
	OBJECT->Transform()->SetRelativeScale(Vec3(81.f, 27.f, 1.f));

	// MeshRender Set
	if (OBJECT->GetRenderComponent() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	wstring wPath;
	
	wPath = L"AlphaBlendMtrl";
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wPath));
	wPath = L"RectMesh";
	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wPath));
	
	OBJECT->MeshRender()->GetDynamicMaterial();

	// PlayerHP�� ����� �ؽ��� ����
	for (UINT i = 0; i < 4; ++i)
	{
		wPath = L"texture\\Battle\\life_" + to_wstring(i) + L".png";
		m_HPTex[i] = CAssetMgr::GetInst()->Load<CTexture>(wPath, wPath);
	}


	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[3]);




}

void CPlayerHP::tick()
{
	// ���� Field�� PlayerHP�� �´� �ؽ��� ���ε�
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[m_pField->GetPlayerHP()]);
}

#undef OBJECT
