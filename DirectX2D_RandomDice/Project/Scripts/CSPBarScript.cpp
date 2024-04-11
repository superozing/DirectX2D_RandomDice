#include "pch.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CDevice.h>

#include "CSPBarScript.h"
#include "CFieldScript.h"
#include "CPracticeModeMgr.h"

CSPBarScript::CSPBarScript()
	:CScript(SPBARSCRIPT)
{
}

CSPBarScript::CSPBarScript(const CSPBarScript& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CSPBarScript::~CSPBarScript()
{
}

#define OBJECT GetOwner()

void CSPBarScript::begin()
{
	assert(OBJECT);

	// �ʵ� ������Ʈ���� ��ũ��Ʈ �޾ƿ���
	m_pFieldScript = m_ModeMgr->GetField();

	//=====================
	// Ʈ������ ������Ʈ ����
	//=====================
	if (OBJECT->Transform() == nullptr)
		OBJECT->AddComponent(new CTransform);

	assert(OBJECT->Transform());

	Vec3 vFieldWorldPos = m_pFieldScript->Transform()->GetRelativePos();

	// -165, -305
	Transform()->SetRelativePos(Vec3(vFieldWorldPos.x - 135, vFieldWorldPos.y - 110, vFieldWorldPos.z - 100));
	Transform()->SetRelativeScale(Vec3(116.f, 36.f, 1.f));


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

	wstrPath = L"texture\\Battle\\icon_sp_bar.png";
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	//==================
	// FONTINFO ���� 
	//==================
	m_fInfo.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo.fFontSize = 20.f;
	m_fInfo.FontType = FONT_TYPE::MAPLE;
	m_fInfo.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fInfo.IsWorldPosRender = true;
}

void CSPBarScript::tick()
{
	Vec3 vPos = OBJECT->Transform()->GetWorldPos();

	// ü�� ��Ʈ�� ��ġ ����
	m_fInfo.fPosX = vPos.x;
	m_fInfo.fPosY = vPos.y;

	m_fInfo.WorldRenderOffset.x = 10;
	m_fInfo.WorldRenderOffset.y = -10;

	// ���� SP�� �ʵ� ��ũ��Ʈ�κ��� ������
	m_fInfo.WStr = to_wstring(m_pFieldScript->GetCurSP());

	// ���� ��Ʈ ����
	CFontMgr::GetInst()->AddRenderFont(m_fInfo);
}

#undef OBJECT