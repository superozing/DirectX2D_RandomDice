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

	// 모드 매니저로부터 필드 스크립트 가져오기
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

	// PlayerHP에 비례한 텍스쳐 세팅
	for (UINT i = 0; i < 4; ++i)
	{
		wPath = L"texture\\Battle\\life_" + to_wstring(i) + L".png";
		m_HPTex[i] = CAssetMgr::GetInst()->Load<CTexture>(wPath, wPath);
	}

	// 기본 텍스쳐 이미지 바인딩
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[3]);

}

void CPlayerHP::tick()
{

	//if (m_pField->GetPlayerHP() == 0)
	// 만약 PlayerHP가 0일 경우, 게임을 종료시켜주어야 한다.
	// 나중에 추가해보아요.

	// 현재 Field의 PlayerHP에 맞는 텍스쳐 바인딩
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_HPTex[m_pField->GetPlayerHP()]);
}

#undef OBJECT
