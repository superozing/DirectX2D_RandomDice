#include "pch.h"
#include "CSummonDiceBtnScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CFieldScript.h"
#include "CUIScript.h"

CSummonDiceBtnScript::CSummonDiceBtnScript()
	: CScript(SUMMONDICEBTNSCRIPT)
{
}

CSummonDiceBtnScript::CSummonDiceBtnScript(const CSummonDiceBtnScript& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CSummonDiceBtnScript::~CSummonDiceBtnScript()
{
}

#define OBJECT GetOwner()

void CSummonDiceBtnScript::begin()
{
	assert(OBJECT);

	// 필드 오브젝트 레벨로부터 가져오기 - 확실하지는 않지만 레이어가 31번이니까 호출이 제일 늦지 않을까요?
	CGameObject* pField = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"FieldObject");
	
	assert(pField);

	// 필드오브젝트에게 스크립트 받아오기
	m_pFieldScript = pField->GetScript<CFieldScript>();

	//=====================
	// 트랜스폼 컴포넌트 설정
	//=====================
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	assert(OBJECT->Transform());

	Vec3 vFieldWorldPos = pField->Transform()->GetWorldPos();

	OBJECT->Transform()->SetRelativePos(Vec3(vFieldWorldPos.x, vFieldWorldPos.y - 100, vFieldWorldPos.z - 100));
	OBJECT->Transform()->SetRelativeScale(Vec3(92.f, 80.f, 1.f));



	//=====================
	// 매쉬렌더 컴포넌트 설정
	//=====================

	// 다른 렌더 컴포넌트를 가진 경우 Assert 처리
	assert(!(OBJECT->GetRenderComponent() != nullptr && OBJECT->MeshRender() == nullptr));

	if (OBJECT->MeshRender() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	assert(OBJECT->MeshRender());

	wstring wstrPath;

	wstrPath = L"RectMesh";
	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wstrPath));

	wstrPath = L"AlphaBlendMtrl";
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wstrPath));

	wstrPath = L"texture\\Battle\\btn_summondice1.png";
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	//=====================
	// UIScript 추가
	//=====================

	m_UI = new CUIScript;
	OBJECT->AddComponent(m_UI);
	m_UI->SetDeletage((CEntity*)m_pFieldScript, (DelegateFunc)&CFieldScript::SummonDice);

}

void CSummonDiceBtnScript::tick()
{
}
