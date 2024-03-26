#include "pch.h"
#include "CDiceUI.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CDevice.h>

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

//      ��������Ʈ ���ÿ� ũ�� ���� �ʿ�
	m_pUI = new CUIScript;
	OBJECT->AddComponent(m_pUI);
	m_pUI->SetDeletage((CEntity*)this, (DelegateFunc)&CDiceUI::ClickLevelUp);




	//==================
	// FONTINFO ���� 
	//==================
	m_FSP1.Color = FONT_RGBA(0, 0, 0, 255);
	m_FSP1.fFontSize = 15.f;
	m_FSP1.FontType = FONT_TYPE::ALBA_SUPER;
	m_FSP1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	  
// 2. ��Ʈ : ������ ���
	m_FSP2.Color = FONT_RGBA(255, 255, 255, 255);
	m_FSP2.fFontSize = 15.5f;
	m_FSP2.FontType = FONT_TYPE::ALBA_MATTER;
	m_FSP2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;

	// ������SP ��Ʈ�� ���ڿ� ����
	m_FSP1.WStr = L"100";
	m_FSP2.WStr = L"100";

// 3. ��Ʈ : ���� ��Ÿ��.
	// ���߿� ������ �ֻ����� �������� �����־�� �Ѵ�.
	m_FDiceLevel.Color = FONT_RGBA(255, 255, 255, 255);
	m_FDiceLevel.fFontSize = 22.f;
	m_FDiceLevel.FontType = FONT_TYPE::EXO2;
	m_FDiceLevel.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FDiceLevel.WStr = L"LV.1";


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

	Vec3 vPos = OBJECT->Transform()->GetWorldPos();
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// ��Ʈ�� ��ġ ����
	m_FSP1.fPosX = vPos.x + (vResol.x / 2) + 12;
	m_FSP1.fPosY = -vPos.y + (vResol.y / 2) + 4;
	m_FSP2.fPosX = vPos.x + (vResol.x / 2) + 12;
	m_FSP2.fPosY = -vPos.y + (vResol.y / 2) + 7;

	m_FDiceLevel.fPosX = vPos.x + (vResol.x / 2);
	m_FDiceLevel.fPosY = -vPos.y + (vResol.y / 2) - 25;

	// ������ ����
	OBJECT->Transform()->SetRelativeScale(m_vScale * m_fScaleSize);

	//// ��Ʈ ����
	CFontMgr::GetInst()->AddRenderFont(m_FSP2);
	CFontMgr::GetInst()->AddRenderFont(m_FSP1);
	//CFontMgr::GetInst()->AddRenderFont(m_FDiceEyeCount);
	CFontMgr::GetInst()->AddRenderFont(m_FDiceLevel);
}									   

void CDiceUI::ClickLevelUp()
{
	UINT NextLevelUpSP = m_pField->DiceLevelUp(m_Dice, m_DeckIdx);

	m_fScaleSize = 0.9f;

	m_FSP1.WStr = to_wstring(NextLevelUpSP);
	m_FSP2.WStr = to_wstring(NextLevelUpSP);

	UINT CurLV = m_pField->GetCurDiceLevel(m_Dice);
	if (CurLV == 5)
		m_FDiceLevel.WStr = L"MAX";
	else
		m_FDiceLevel.WStr = L"LV." + to_wstring(CurLV);
}

#undef OBJECT