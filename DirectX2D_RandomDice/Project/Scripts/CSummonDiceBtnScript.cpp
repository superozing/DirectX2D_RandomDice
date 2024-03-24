#include "pch.h"
#include "CSummonDiceBtnScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CFieldScript.h"
#include "CUIScript.h"
#include <Engine/CDevice.h>

CSummonDiceBtnScript::CSummonDiceBtnScript()
	: CScript(SUMMONDICEBTNSCRIPT)
	, m_fScaleSize(1.f)
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
	m_vScale = Vec3(92.f, 80.f, 1.f);
	OBJECT->Transform()->SetRelativeScale(m_vScale);


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
	m_UI->SetDeletage((CEntity*)this, (DelegateFunc)&CSummonDiceBtnScript::ClickButton);



	//==================
	// FONTINFO 세팅 
	//==================
	m_fInfo1.Color = FONT_RGBA(0, 0, 0, 255);
	m_fInfo1.fFontSize = 12.f;
	m_fInfo1.FontType = FONT_TYPE::ALBA_SUPER;
	m_fInfo1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;

	m_fInfo2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo2.fFontSize = 12.f;
	m_fInfo2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fInfo2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;

	// 체력 폰트의 문자열 설정
	m_fInfo1.WStr = L"10";
	m_fInfo2.WStr = L"10";

}

void CSummonDiceBtnScript::ClickButton()
{
	m_pFieldScript->SummonDice();

	m_fScaleSize = 0.9f;
	
	m_fInfo1.WStr = to_wstring(m_pFieldScript->GetSummonSP());
	m_fInfo2.WStr = to_wstring(m_pFieldScript->GetSummonSP());


}


void CSummonDiceBtnScript::tick()
{
	m_fScaleSize += 0.8f * DT;
	
	if (m_fScaleSize > 1.f)
		m_fScaleSize = 1.f;

	// 스케일 적용
	OBJECT->Transform()->SetRelativeScale(m_vScale * m_fScaleSize);

	Vec3 vPos = OBJECT->Transform()->GetWorldPos();
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// 체력 폰트의 위치 설정
	m_fInfo1.fPosX = vPos.x + (vResol.x / 2);
	m_fInfo1.fPosY = -vPos.y + (vResol.y / 2) + 9;
	m_fInfo2.fPosX = vPos.x + (vResol.x / 2);
	m_fInfo2.fPosY = -vPos.y + (vResol.y / 2) + 12;

	// 폰트 매니저 출력에 추가
	CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
	CFontMgr::GetInst()->AddRenderFont(m_fInfo1);
}

