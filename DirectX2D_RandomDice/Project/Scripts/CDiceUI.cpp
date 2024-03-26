#include "pch.h"
#include "CDiceUI.h"

#include <Engine\CAssetMgr.h>

#include "CUIScript.h"



CDiceUI::CDiceUI()
	:CScript(DICEUI)
	, m_fScaleSize(1.f)
{
}

CDiceUI::CDiceUI(const CDiceUI& _Origin)
	:CScript(_Origin.GetScriptType())
	, m_fScaleSize(1.f)
{
}

CDiceUI::~CDiceUI()
{
}

#define OBJECT GetOwner()


void CDiceUI::begin()
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"�ֻ��� ����", &m_Dice);

	assert(OBJECT);

	// �⺻���� ����
	
	// Ʈ������ ����
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	OBJECT->Transform()->SetRelativeScale(Vec3(118.f, 118.f, 1.f));
	m_vScale = Vec3(118.f, 118.f, 1.f);
	// ������ Ʈ�������� ��ġ �� ����
	OBJECT->Transform()->SetRelativePos(Vec3(-180.f + (90.f * m_DeckIdx), -210.f, -100.f));

	// �Ž����� ����
	if (OBJECT->MeshRender() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	// �Ž�, ���͸��� ����
	wstring wstrPath;

	wstrPath = L"RectMesh"; 
	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wstrPath));

	wstrPath = L"AlphaBlendMtrl";
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wstrPath));

	OBJECT->MeshRender()->GetDynamicMaterial();

	// �� ������ Dice ����


	// DICE�� �´� ��� �����ͼ� �ؽ��� ���ε�
	wstrPath = CDiceScript::DicePath[(UINT)m_Dice];
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

    // ���� �ʿ��ұ�?
// 1. UI->������ ��ư ������ ����. O

//      ��������Ʈ ���ÿ� ũ�� ���� �ʿ�



	m_pUI = new CUIScript;
	OBJECT->AddComponent(m_pUI);
	m_pUI->SetDeletage((CEntity*)this, (DelegateFunc)&CDiceUI::ClickLevelUp);





// 2. ��Ʈ : ������ ���
// 3. ��Ʈ : ���� ��Ÿ��.
// 4. ��׶��� �̹���(���� ������Ʈ): ���� �� ���� ��׶���
//		1) ��ƼŬ �մ� ������Ʈ
// 	    2) �ֻ����� ���� ���� ��Ÿ�� ��Ʈ
// 5. ��׶��� �̹���(���� ������Ʈ):(SP) <- �̰�.
}

void CDiceUI::tick()
{
	// ũ�� ����
	m_fScaleSize += 0.8f * DT;

	if (m_fScaleSize > 1.f)
		m_fScaleSize = 1.f;

	// ������ ����
	OBJECT->Transform()->SetRelativeScale(m_vScale * m_fScaleSize);

	//// ��Ʈ ����
	//CFontMgr::GetInst()->AddRenderFont(m_FSP1);
	//CFontMgr::GetInst()->AddRenderFont(m_FSP2);
	//CFontMgr::GetInst()->AddRenderFont(m_FDiceEyeCount);
	//CFontMgr::GetInst()->AddRenderFont(m_FDiceLevel);
}									   

void CDiceUI::ClickLevelUp()
{
	m_pField->DiceLevelUp(m_Dice);

	m_fScaleSize = 0.9f;

	//m_fInfo1.WStr = to_wstring(m_pFieldScript->GetSummonSP());
	//m_fInfo2.WStr = to_wstring(m_pFieldScript->GetSummonSP());
}

#undef OBJECT