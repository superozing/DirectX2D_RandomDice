#include "pch.h"
#include "CDiceScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

// 정의
vector<wstring> CDiceScript::DicePath;

CDiceScript::CDiceScript()
	:CScript(DICESCRIPT)
	,m_fScaleSize(1.f)
{

}

CDiceScript::~CDiceScript()
{


}

#define OBJECT GetOwner()

void CDiceScript::SetDiceType(DICE _Dice)
{
	//if (Dice != DICE::NONE)
	// 이 때 점점 커지면서 파티클 방출 해야해요.

	m_Dice = _Dice;

	if (m_Dice == DICE::NONE)
	{
		m_fScaleSize = 0.f;
		m_IsGrowing = false;
		return;
	}

	m_IsGrowing = true;

	m_SpawnParticle->SetActivate(true);

	wstring wstrPath = DicePath[(UINT)m_Dice];

	OBJECT->GetRenderComponent()->GetDynamicMaterial()
		->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
}

void CDiceScript::SetDiceInfo(DICE _Dice)
{

}

void CDiceScript::SetDiceXY(UINT _DiceRow, UINT _DiceCol)
{
	m_DiceRow = _DiceRow;
	m_DiceCol = _DiceCol;

	Vec3 fieldPos = m_OwnerField->Transform()->GetRelativePos();


	OBJECT->SetName(L"Dice_" + to_wstring(m_DiceRow) + L"_" + to_wstring(m_DiceCol));

	Vec3 vvfieldPos = OBJECT->Transform()->GetRelativePos();

}

void CDiceScript::begin()
{
	if (OBJECT == nullptr)
		return;

	m_vSrcScale = Vec3(90.f, 90.f, 1.f);

	if (OBJECT->MeshRender() != nullptr)
		OBJECT->GetRenderComponent()->GetDynamicMaterial();


	// 주사위 생성 파티클 모듈
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -500.f));

	m_SpawnParticle = new CParticleSystem;
	pObject->AddComponent(m_SpawnParticle);

	tParticleModule tModule;

	// 초기 모듈 세팅		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;
	
	tModule.SpaceType = 1;
	
	tModule.vSpawnColor = Vec4(1.f, 1.f, 1.f, 0.6f);
	tModule.vSpawnMinScale = Vec4(10.f, 10.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(20.f, 20.f, 1.f, 1.f);
	
	tModule.MinLife = 0.7f;
	tModule.MaxLife = 1.f;
	tModule.MinMass = 1.f;
	tModule.MaxMass = 1.f;
	tModule.SpawnShape = 0; // 0 : Sphere, 1 : Box
	tModule.Radius = 40.f;
	tModule.SpawnRate = 100;
	
	// Add Velocity Module
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	tModule.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	tModule.MinSpeed = 70;
	tModule.MaxSpeed = 70;
	
	// Noise Force
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	tModule.NoiseForceScale = 10.f;
	tModule.NoiseForceTerm = 0.3f;
	
	// Render 
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	tModule.VelocityAlignment = 1; // 속도에 따른 방향 정렬
	tModule.AlphaBasedLife = 0; // 0 : off, 1 : NomrlizedAge, 2: Age
	tModule.AlphaMaxAge = 2.f;
	
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	tModule.DragTime = 1.f;

	wstring wPath = L"texture\\particle\\diceSummon.png";
	pObject->SetName(L"ParticleObject");
	m_SpawnParticle->SetParticleModule(tModule);
	m_SpawnParticle->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(wPath, wPath));

	//CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObject, 6);
	OBJECT->AddChild(pObject);
}

void CDiceScript::tick()
{

	//if (OBJECT->GetChild().size() == 0)
	//	OBJECT->AddChild(m_SpawnParticle->GetOwner());

	if (m_IsGrowing)
	{
		m_fScaleSize += 5.f * DT;
		if (m_fScaleSize > 0.5f)
			m_SpawnParticle->SetActivate(false);

		if (m_fScaleSize > 1.f)
		{
			m_fScaleSize = 1.f;
			m_IsGrowing = false;
		}
	}
	OBJECT->Transform()->SetRelativeScale(m_vSrcScale * m_fScaleSize);


	OBJECT->GetRenderComponent()->GetDynamicMaterial()
		->SetScalarParam(SCALAR_PARAM::FLOAT_0, m_fScaleSize);
}

void CDiceScript::InitDicePath()
{
	CDiceScript::DicePath.resize((UINT)DICE::END);

	CDiceScript::DicePath[(UINT)DICE::NONE] = L"NONE";

	CDiceScript::DicePath[(UINT)DICE::FIRE] = L"texture\\Dice\\01_fire.png";
	CDiceScript::DicePath[(UINT)DICE::ELECTRIC] = L"texture\\Dice\\02_electric.png";
	CDiceScript::DicePath[(UINT)DICE::WIND] = L"texture\\Dice\\03_wind.png";
	CDiceScript::DicePath[(UINT)DICE::POISON] = L"texture\\Dice\\04_poison.png";
	CDiceScript::DicePath[(UINT)DICE::ICE] = L"texture\\Dice\\05_ice.png";
	CDiceScript::DicePath[(UINT)DICE::IRON] = L"texture\\Dice\\06_iron.png";
	CDiceScript::DicePath[(UINT)DICE::BROKEN] = L"texture\\Dice\\07_broken.png";
	CDiceScript::DicePath[(UINT)DICE::GAMBLE] = L"texture\\Dice\\08_gamble.png";
	CDiceScript::DicePath[(UINT)DICE::LOCK] = L"texture\\Dice\\09_lock.png";
	CDiceScript::DicePath[(UINT)DICE::MINE] = L"texture\\Dice\\10_mine.png";
	CDiceScript::DicePath[(UINT)DICE::LIGHT] = L"texture\\Dice\\11_light.png";
	CDiceScript::DicePath[(UINT)DICE::THORN] = L"texture\\Dice\\12_thorn.png";
	CDiceScript::DicePath[(UINT)DICE::CRACK] = L"texture\\Dice\\13_crack.png";
	CDiceScript::DicePath[(UINT)DICE::CRITICAL] = L"texture\\Dice\\14_critical.png";
	CDiceScript::DicePath[(UINT)DICE::ENERGY] = L"texture\\Dice\\15_energy.png";
	CDiceScript::DicePath[(UINT)DICE::SACRIFICE] = L"texture\\Dice\\16_sacrifice.png";
	CDiceScript::DicePath[(UINT)DICE::ARROW] = L"texture\\Dice\\17_arrow.png";
	CDiceScript::DicePath[(UINT)DICE::MIMIC] = L"texture\\Dice\\18_mimic.png";
}


#undef OBJECT