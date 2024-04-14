#include "pch.h"
#include "CSilenceBoss.h"

CSilenceBoss::CSilenceBoss()
	: CBossScript(SILENCEBOSS)
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BOSS);

	tParticleModule tModule;

	// 초기 모듈 세팅		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 1;

	tModule.vSpawnColor = Vec4(0.2f, 0.2f, 0.2f, 0.6f);
	tModule.vSpawnMinScale = Vec4(70.f, 70.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(150.f, 150.f, 1.f, 1.f);

	tModule.MinLife = 0.5f;
	tModule.MaxLife = 1.f;
	tModule.MinMass = 1.f;
	tModule.MaxMass = 1.f;
	tModule.SpawnShape = 0; // 0 : Sphere, 1 : Box
	tModule.Radius = 10.f;
	tModule.SpawnRate = 50;

	// Add Velocity Module
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	tModule.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	tModule.MinSpeed = 70;
	tModule.MaxSpeed = 40;
	tModule.FixedDirection;
	tModule.FixedAngle;

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

	ENEMY_INFO tInfo;

	tInfo.DeathParticleModule = tModule;

	tInfo.MonTex = TEX_LOAD(L"texture\\Enemy\\Boss\\boss_2.png");
	tInfo.MoveSpeed = 5.f; // 초당 진행도 5 증가. 총 20초가 걸려야 끝 지점에 도달해요.
	tInfo.MonScale = Vec3(100.f, 100.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);
}

CSilenceBoss::CSilenceBoss(const CSilenceBoss& _Origin)
	: CBossScript((SCRIPT_TYPE)_Origin.GetScriptType())
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BOSS);

	tParticleModule tModule;

	// 초기 모듈 세팅		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 1;

	tModule.vSpawnColor = Vec4(0.2f, 0.2f, 0.2f, 0.6f);
	tModule.vSpawnMinScale = Vec4(70.f, 70.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(150.f, 150.f, 1.f, 1.f);

	tModule.MinLife = 0.5f;
	tModule.MaxLife = 1.f;
	tModule.MinMass = 1.f;
	tModule.MaxMass = 1.f;
	tModule.SpawnShape = 0; // 0 : Sphere, 1 : Box
	tModule.Radius = 10.f;
	tModule.SpawnRate = 50;

	// Add Velocity Module
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = 1;
	tModule.AddVelocityType = 0; // 0 : From Center, 1: To Center, 2: Fix Direction
	tModule.MinSpeed = 70;
	tModule.MaxSpeed = 40;
	tModule.FixedDirection;
	tModule.FixedAngle;

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

	ENEMY_INFO tInfo;

	tInfo.DeathParticleModule = tModule;

	tInfo.MonTex = TEX_LOAD(L"texture\\Enemy\\Boss\\boss_2.png");
	tInfo.MoveSpeed = 5.f; // 초당 진행도 5 증가. 총 20초가 걸려야 끝 지점에 도달해요.
	tInfo.MonScale = Vec3(100.f, 100.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);
}

CSilenceBoss::~CSilenceBoss()
{
}

void CSilenceBoss::BossSkill()
{
}

void CSilenceBoss::begin()
{
	CEnemyScript::begin();
}

void CSilenceBoss::tick()
{
	CEnemyScript::tick();

}
