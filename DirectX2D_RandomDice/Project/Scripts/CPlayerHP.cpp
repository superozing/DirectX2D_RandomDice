#include "pch.h"
#include "CPlayerHP.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CFieldScript.h"
#include "CPracticeModeMgr.h"

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
	assert(OBJECT);

	// ��� �Ŵ����κ��� �ʵ� ��ũ��Ʈ ��������
	m_pField = m_ModeMgr->GetField();

	// Transform Set
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	OBJECT->Transform()->SetRelativePos(m_PlayerHPPos);
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

	// �⺻ �ؽ��� �̹��� ���ε�
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[3]);

}

void CPlayerHP::tick()
{

	//if (m_pField->GetPlayerHP() == 0)
	// ���� PlayerHP�� 0�� ���, ������ ��������־�� �Ѵ�.
	// ���߿� �߰��غ��ƿ�.

	// ���� Field�� PlayerHP�� �´� �ؽ��� ���ε�
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[m_pField->GetPlayerHP()]);
}

#undef OBJECT
