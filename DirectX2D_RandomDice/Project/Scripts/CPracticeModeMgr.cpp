#include "pch.h"
#include "CPracticeModeMgr.h"

#include "CFieldScript.h"
#include "CSummonDiceBtnScript.h"
#include "CSPBarScript.h"

CPracticeModeMgr::CPracticeModeMgr()
	: CScript(PRACTICEMODEMGR)
{
}

CPracticeModeMgr::CPracticeModeMgr(const CPracticeModeMgr& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CPracticeModeMgr::~CPracticeModeMgr()
{
}

#define OBJ GetOwner()

void CPracticeModeMgr::begin()
{
	OBJ->SetName(L"PracticeModeMgr");

	// Transform set
	if (Transform() == nullptr)
		OBJ->AddComponent(new CTransform);

	Transform()->SetRelativePos(Vec3(0.f, 80.f, 0.f));
	Transform()->SetRelativeScale(Vec3(540.f, 40.f, 1.f));


	// MeshRender set
	if (GetRenderComponent() == nullptr)
		OBJ->AddComponent(new CMeshRender);

	MeshRender()->SetMesh(ASSET_MGR->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(ASSET_MGR->FindAsset<CMaterial>(L"AlphaBlendMtrl"));

	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, TEX_LOAD(L"texture\\Battle\\ingame_blackbox1.png"));



	CGameObject* pObj;

	//===========
	// Field 
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"FieldObject");
	m_Field = new CFieldScript;
	pObj->AddComponent(m_Field);
	OBJ->AddChild(pObj);
	m_Field->LoadDeckInfoFromFile();
	m_Field->SetFieldPos(Vec3(0.f, -290.f, 1000.f));
	m_Field->SetModeMgr(this);

	//===========
	// SummonBtn 
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"SummonBtn");
	m_SummonBtn = new CSummonDiceBtnScript;
	pObj->AddComponent(m_SummonBtn);
	OBJ->AddChild(pObj);
	m_SummonBtn->SetDiceBtnPos(Vec3(0.f, -390.f, 900.f));
	m_SummonBtn->SetModeMgr(this);

	//===========
	// SP Bar
	//===========
	pObj = new CGameObject;
	pObj->SetName(L"SPBar");
	m_SPBar = new CSPBarScript;
	pObj->AddComponent(m_SPBar);
	OBJ->AddChild(pObj);
	m_SPBar->SetModeMgr(this);

}

void CPracticeModeMgr::tick()
{
}

#undef OBJ
