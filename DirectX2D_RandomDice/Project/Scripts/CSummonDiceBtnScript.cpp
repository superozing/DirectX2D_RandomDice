#include "pch.h"
#include "CSummonDiceBtnScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine/CDevice.h>

#include "CFieldScript.h"
#include "CUIScript.h"
#include "CPracticeModeMgr.h"

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

	// �ʵ������Ʈ���� ��ũ��Ʈ �޾ƿ���
	m_pFieldScript = m_ModeMgr->GetField();

	//=====================
	// Ʈ������ ������Ʈ ����
	//=====================
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	Vec3 vFieldWorldPos = m_pFieldScript->Transform()->GetWorldPos();

	OBJECT->Transform()->SetRelativePos(m_SummonDiceBtnPos);
	m_vScale = Vec3(92.f, 80.f, 1.f);
	OBJECT->Transform()->SetRelativeScale(m_vScale);


	//=====================
	// �Ž����� ������Ʈ ����
	//=====================

	// �ٸ� ���� ������Ʈ�� ���� ��� Assert ó��
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
	// UIScript �߰�
	//=====================

	m_UI = new CUIScript;
	OBJECT->AddComponent(m_UI);
	m_UI->SetDeletage((CEntity*)this, (DelegateFunc)&CSummonDiceBtnScript::ClickButton);



	//==================
	// FONTINFO ���� 
	//==================
	m_fInfo1.Color = FONT_RGBA(0, 0, 0, 255);
	m_fInfo1.fFontSize = 12.f;
	m_fInfo1.FontType = FONT_TYPE::ALBA_SUPER;
	m_fInfo1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fInfo1.IsWorldPosRender = true;

	m_fInfo2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo2.fFontSize = 12.f;
	m_fInfo2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fInfo2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fInfo1.IsWorldPosRender = true;

	// ü�� ��Ʈ�� ���ڿ� ����
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

	// ������ ����
	OBJECT->Transform()->SetRelativeScale(m_vScale * m_fScaleSize);

	Vec3 vPos = OBJECT->Transform()->GetWorldPos();

	// ü�� ��Ʈ�� ��ġ ����
	m_fInfo1.fPosX = vPos.x;
	m_fInfo1.fPosY = vPos.y;
	m_fInfo1.WorldRenderOffset.y = 9;

	m_fInfo2.fPosX = vPos.x;
	m_fInfo2.fPosY = vPos.y;
	m_fInfo2.WorldRenderOffset.y = 12;

	// ��Ʈ �Ŵ��� ��¿� �߰�
	CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
	CFontMgr::GetInst()->AddRenderFont(m_fInfo1);


	if (m_pFieldScript->GetCurSP() < m_pFieldScript->GetSummonSP() || m_pFieldScript->GetDiceCount() == 15)
		OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);
	else
		OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
}

