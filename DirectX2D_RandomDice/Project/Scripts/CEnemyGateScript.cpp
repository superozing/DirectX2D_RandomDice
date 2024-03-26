#include "pch.h"
#include "CEnemyGateScript.h"
#include "CRotateScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CEnemyGateScript::CEnemyGateScript()
	: CScript(SCRIPT_TYPE::ENEMYGATESCRIPT)
{
}

CEnemyGateScript::CEnemyGateScript(const CEnemyGateScript& _Origin)
	: CScript(_Origin.GetScriptType())
{
}

CEnemyGateScript::~CEnemyGateScript()
{
}

#define OBJECT GetOwner()

void CEnemyGateScript::begin()
{
	assert(OBJECT);

	wstring wstrPath;

	OBJECT->Transform()->SetRelativeScale(Vec3(38.f, 38.f, 1.f));
	
	// MeshRender - Mesh, Material, Texture Set
	OBJECT->AddComponent(new CMeshRender);

	wstrPath = L"RectMesh";
	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(wstrPath));

	wstrPath = L"AlphaBlendMtrl";
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(wstrPath));

	wstrPath = L"texture\\BattleField\\ingame_mob_gate.png";
	OBJECT->MeshRender()->GetDynamicMaterial()
		->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	// CRotateScript �߰�
	CRotateScript* rotScript = new CRotateScript;
	OBJECT->AddComponent(rotScript);
	rotScript->SetRotateValue(Vec3(0.f, 0.f, -XM_PI));








	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	Ptr<CPrefab> pObjPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	// ���ĺ��� ���ӿ�����Ʈ ������ ��üȭ
	m_Background = pObjPref->Instantiate();
	
	// �̸� ����
	wstrPath = L"texture\\BattleField\\ingame_mob_gate_in_1_1.png";
	m_Background->SetName(wstrPath);

	// ���� ���� ����, �ؽ��� ���ε�
	m_Background->MeshRender()->GetDynamicMaterial()->
		SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	// Transform()�� Scale, ����Ʈ�� Pos Set
	Vec3 gatePos(0.f, 0.f, 0.f);
	gatePos.z += 1;
	m_Background->Transform()->SetRelativePos(gatePos);
	m_Background->Transform()->SetRelativeScale(Vec3(44.f, 44.f, 1.f));

	// AddObject
	OBJECT->AddChild(m_Background);



	/////////////////////
	// Add ParticleSystem
	/////////////////////
	

	// ��ƼŬ ������Ʈ ����
	m_pParticleObject = new CGameObject;
	m_pParticleObject->AddComponent(new CTransform);

	gatePos.z -= 2;

	m_pParticleObject->Transform()->SetRelativePos(gatePos);

	// ��ƼŬ �ý��� ������Ʈ ����
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// ��ƼŬ ������Ʈ�� ��ƼŬ �ý��� ����

	tParticleModule tModule;

	// �ʱ� ��� ����		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 0;

	tModule.vSpawnColor = Vec4(1.f, 1.f, 1.f, 1.f);
	tModule.vSpawnMinScale = Vec4(1.f, 1.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(8.f, 8.f, 1.f, 1.f);

	tModule.MinLife = 1.f;
	tModule.MaxLife = 1.5f;
	tModule.MinMass = 1.f;
	tModule.MaxMass = 1.f;
	tModule.SpawnShape = 0; // 0 : Sphere, 1 : Box
	tModule.Radius = 25.f;
	tModule.SpawnRate = 4;

	// Add Velocity Module
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	tModule.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	tModule.MinSpeed = 40;
	tModule.MaxSpeed = 40;
	tModule.FixedDirection;

	// Noise Force
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = 1;
	tModule.NoiseForceScale = 10.f;
	tModule.NoiseForceTerm = 0.3f;

	// Render 
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = 1;
	tModule.VelocityAlignment = 1; // �ӵ��� ���� ���� ����
	tModule.AlphaBasedLife = 0; // 0 : off, 1 : NomrlizedAge, 2: Age
	tModule.AlphaMaxAge = 2.f;

	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	tModule.DragTime = 1.f;

	m_ParticleSystem->SetParticleModule(tModule);

	wstrPath = L"texture\\particle\\ingame_mob_gate_ef2.png";
	m_ParticleSystem->SetName(wstrPath);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
	m_ParticleSystem->SetActivate(true);


	// ��ƼŬ ������Ʈ �߰�
	OBJECT->AddChild(m_pParticleObject);

}


#undef OBJECT
