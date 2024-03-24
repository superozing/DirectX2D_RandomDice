#include "pch.h"
#include "CSummonDiceBtnScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#include "CFieldScript.h"
#include "CUIScript.h"
#include <Engine/CDevice.h>

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

	// �ʵ� ������Ʈ �����κ��� �������� - Ȯ�������� ������ ���̾ 31���̴ϱ� ȣ���� ���� ���� �������?
	CGameObject* pField = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"FieldObject");

	assert(pField);

	// �ʵ������Ʈ���� ��ũ��Ʈ �޾ƿ���
	m_pFieldScript = pField->GetScript<CFieldScript>();

	//=====================
	// Ʈ������ ������Ʈ ����
	//=====================
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	assert(OBJECT->Transform());

	Vec3 vFieldWorldPos = pField->Transform()->GetWorldPos();

	OBJECT->Transform()->SetRelativePos(Vec3(vFieldWorldPos.x, vFieldWorldPos.y - 100, vFieldWorldPos.z - 100));
	OBJECT->Transform()->SetRelativeScale(Vec3(92.f, 80.f, 1.f));



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

	m_fInfo2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo2.fFontSize = 12.f;
	m_fInfo2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fInfo2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;

	// ü�� ��Ʈ�� ���ڿ� ����
	m_fInfo1.WStr = L"10";
	m_fInfo2.WStr = L"10";

}

void CSummonDiceBtnScript::ClickButton()
{
	m_pFieldScript->SummonDice();
	
	m_fInfo1.WStr = to_wstring(m_pFieldScript->GetSummonSP());
	m_fInfo2.WStr = to_wstring(m_pFieldScript->GetSummonSP());


}




void CSummonDiceBtnScript::tick()
{
	Vec3 vPos = OBJECT->Transform()->GetWorldPos();
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// ü�� ��Ʈ�� ��ġ ����
	m_fInfo1.fPosX = vPos.x + (vResol.x / 2);
	m_fInfo1.fPosY = -vPos.y + (vResol.y / 2) + 11;
	m_fInfo2.fPosX = vPos.x + (vResol.x / 2);
	m_fInfo2.fPosY = -vPos.y + (vResol.y / 2) + 14;

	// ��Ʈ �Ŵ��� ��¿� �߰�
	CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
	CFontMgr::GetInst()->AddRenderFont(m_fInfo1);
}

