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


	// 4. ��׶��� �̹���(���� ������Ʈ): ���� �� ���� ��׶���
	CGameObject* pObj = new CGameObject;

	OBJECT->AddChild(pObj);

	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(30.f, 30.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(25.f, 15.f, 1.f));

	pObj->AddComponent(new CMeshRender);
	wstrPath = L"AlphaBlendMtrl";
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wstrPath));

	wstrPath = L"RectMesh";
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wstrPath));

	wstrPath = L"texture\\Battle\\ingame_dice_eye_back.png";
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

		//	1) ��ƼŬ �մ� ������Ʈ
	
	tParticleModule tModule;

	// �ʱ� ��� ����		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 0;

	tModule.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	tModule.vSpawnMinScale = Vec4(8.f, 8.f, 8.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(14.f, 14.f, 14.f, 1.f);

	tModule.MinLife = 1.f;
	tModule.MaxLife = 1.f;
	tModule.MinMass = 1.f;
	tModule.MaxMass = 1.f;
	tModule.SpawnShape = 1; // 0 : Sphere, 1 : Box
	tModule.Radius = 0.f;
	tModule.vSpawnBoxScale = Vec4(20, 1, 20, 1);	// SpawnShape �� Box �� ���, Box �� ũ��
	tModule.SpawnRate = 2;

	// Add Velocity Module
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	tModule.AddVelocityType = 2; // 0 : From Center, 1: To Center, 2: Fix Direction
	tModule.MinSpeed = 30;
	tModule.MaxSpeed = 30;
	tModule.FixedDirection;
	tModule.FixedAngle = 90.f;

	// Noise Force
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	tModule.NoiseForceScale = 10.f;
	tModule.NoiseForceTerm = 0.3f;

	// Render 
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	tModule.VelocityAlignment = 1; // �ӵ��� ���� ���� ����
	tModule.AlphaBasedLife = 0; // 0 : off, 1 : NomrlizedAge, 2: Age
	tModule.AlphaMaxAge = 2.f;

	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 0;
	tModule.DragTime = 2.f;

	//////
	pObj = new CGameObject;

	OBJECT->AddChild(pObj);

	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(30.f, 30.f, -11.f));
	pObj->Transform()->SetRelativeScale(Vec3(25.f, 15.f, 1.f));
	pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 90.f));

	CParticleSystem* pParticle = new CParticleSystem;
	pObj->AddComponent(pParticle);

	wstrPath = L"texture\\particle\\ingame_mob_gate_ef2.png";
	pParticle->SetParticleModule(tModule);
	pParticle->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	pParticle->SetActivate(true);



	// 5. ��׶��� �̹���(���� ������Ʈ):(SP) <- �̰�.
		// SP �̹��� ����� ����...
	pObj = new CGameObject;

	OBJECT->AddChild(pObj);

	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(-18.f, -19.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 1.f));
	
	pObj->AddComponent(new CMeshRender);
	wstrPath = L"AlphaBlendMtrl";
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wstrPath));
	
	wstrPath = L"RectMesh";
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wstrPath));

	wstrPath = L"texture\\Battle\\icon_sp_1.png";
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));



	//==================
	// FONTINFO ���� 
	//==================
// 2. ��Ʈ : ������ ���
	m_FSP1.Color = FONT_RGBA(0, 0, 0, 255);
	m_FSP1.fFontSize = 15.f;
	m_FSP1.FontType = FONT_TYPE::ALBA_SUPER;
	m_FSP1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FSP1.IsWorldPosRender = true;
	  
	m_FSP2.Color = FONT_RGBA(255, 255, 255, 255);
	m_FSP2.fFontSize = 15.5f;
	m_FSP2.FontType = FONT_TYPE::ALBA_MATTER;
	m_FSP2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FSP2.IsWorldPosRender = true;

	// ������SP ��Ʈ�� ���ڿ� ����
	m_FSP1.WStr = L"100";
	m_FSP2.WStr = L"100";

// 	2) �ֻ����� ���� ���� ��Ÿ�� ��Ʈ
	m_FDiceEyeCount1.Color;
	m_FDiceEyeCount2.Color; 
	m_FDiceEyeCount1.Color = FONT_RGBA(0, 0, 0, 255);
	m_FDiceEyeCount1.fFontSize = 14.f;
	m_FDiceEyeCount1.FontType = FONT_TYPE::ALBA_SUPER;
	m_FDiceEyeCount1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FDiceEyeCount1.IsWorldPosRender = true;

	// 2. ��Ʈ : ������ ���
	m_FDiceEyeCount2.Color = FONT_RGBA(255, 255, 255, 255);
	m_FDiceEyeCount2.fFontSize = 14.2f;
	m_FDiceEyeCount2.FontType = FONT_TYPE::ALBA_MATTER;
	m_FDiceEyeCount2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FDiceEyeCount2.IsWorldPosRender = true;

	m_FDiceEyeCount1.WStr = L"0";
	m_FDiceEyeCount2.WStr = L"0";

// 3. ��Ʈ : ���� ��Ÿ��.
	// ���߿� ������ �ֻ����� �������� �����־�� �Ѵ�.
	m_FDiceLevel.Color = FONT_RGBA(255, 255, 255, 255);
	m_FDiceLevel.fFontSize = 22.f;
	m_FDiceLevel.FontType = FONT_TYPE::EXO2;
	m_FDiceLevel.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_FDiceLevel.IsWorldPosRender = true;

	m_FDiceLevel.WStr = L"LV.1";



	// ��Ʈ ������ ����


}

void CDiceUI::tick()
{
	// ũ�� ����
	m_fScaleSize += 0.8f * DT;

	if (m_fScaleSize > 1.f)
		m_fScaleSize = 1.f;

	Vec3 vPos = OBJECT->Transform()->GetWorldPos();

	// ��Ʈ�� ��ġ ����
	m_FSP1.fPosX = vPos.x;
	m_FSP1.fPosY = vPos.y;
	m_FSP2.fPosX = vPos.x;
	m_FSP2.fPosY = vPos.y;

	m_FSP1.WorldRenderOffset.x = 12;
	m_FSP1.WorldRenderOffset.y = 4;
	m_FSP2.WorldRenderOffset.x = 12;
	m_FSP2.WorldRenderOffset.y = 7;

	///

	m_FDiceEyeCount1.fPosX = vPos.x;
	m_FDiceEyeCount1.fPosY = vPos.y;
	m_FDiceEyeCount2.fPosX = vPos.x;
	m_FDiceEyeCount2.fPosY = vPos.y;

	m_FDiceEyeCount1.WorldRenderOffset.x = 30;
	m_FDiceEyeCount1.WorldRenderOffset.y = -47;
	m_FDiceEyeCount2.WorldRenderOffset.x = 30;
	m_FDiceEyeCount2.WorldRenderOffset.y = -44;

	///

	m_FDiceLevel.fPosX = vPos.x;
	m_FDiceLevel.fPosY = vPos.y;

	m_FDiceLevel.WorldRenderOffset.y = -25;

	// ������ ����
	OBJECT->Transform()->SetRelativeScale(m_vScale * m_fScaleSize);

	//// ��Ʈ ����
	CFontMgr::GetInst()->AddRenderFont(m_FSP2);
	CFontMgr::GetInst()->AddRenderFont(m_FSP1);
	CFontMgr::GetInst()->AddRenderFont(m_FDiceEyeCount2);
	CFontMgr::GetInst()->AddRenderFont(m_FDiceEyeCount1);
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
	{
		m_FDiceLevel.WStr = L"MAX";
		OBJECT->MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.7f);
		m_FSP2.Color = FONT_RGBA(200, 200, 200, 255);
		m_FDiceLevel.Color = FONT_RGBA(200, 200, 200, 255);

	}
	else
		m_FDiceLevel.WStr = L"LV." + to_wstring(CurLV);
}

#undef OBJECT