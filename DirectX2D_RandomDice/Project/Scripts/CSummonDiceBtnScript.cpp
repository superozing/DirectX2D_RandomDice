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
	m_UI->SetDeletage((CEntity*)m_pFieldScript, (DelegateFunc)&CFieldScript::SummonDice);

}

void CSummonDiceBtnScript::tick()
{
}
