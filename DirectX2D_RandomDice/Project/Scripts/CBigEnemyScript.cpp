#include "pch.h"
#include "CBigEnemyScript.h"

CBigEnemyScript::CBigEnemyScript()
	:CEnemyScript(SCRIPT_TYPE::BIGENEMYSCRIPT)
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BIG);

	tParticleModule tModule;

	// 초기 모듈 세팅		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 1;

	tModule.vSpawnColor = Vec4(0.2f, 0.2f, 0.2f, 0.6f);
	tModule.vSpawnMinScale = Vec4(40.f, 40.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(70.f, 70.f, 1.f, 1.f);

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

	wstring strPath = L"texture\\Enemy\\enemy_big.png";

	tInfo.MonTex = CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath);
	tInfo.MoveSpeed = 5.f; // 초당 진행도 5 증가. 총 20초가 걸려야 끝 지점에 도달해요.
	tInfo.MonScale = Vec3(80.f, 80.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);

}

CBigEnemyScript::CBigEnemyScript(const CBigEnemyScript& _Origin)
	:CEnemyScript(SCRIPT_TYPE::BIGENEMYSCRIPT)
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BIG);

	tParticleModule tModule;

	// 초기 모듈 세팅		
	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::SPAWN] = 1;

	tModule.SpaceType = 1;

	tModule.vSpawnColor = Vec4(0.2f, 0.2f, 0.2f, 0.6f);
	tModule.vSpawnMinScale = Vec4(40.f, 40.f, 1.f, 1.f);
	tModule.vSpawnMaxScale = Vec4(70.f, 70.f, 1.f, 1.f);

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

	wstring strPath = L"texture\\Enemy\\enemy_big.png";

	tInfo.MonTex = CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath);
	tInfo.MoveSpeed = 5.f; // 초당 진행도 5 증가. 총 20초가 걸려야 끝 지점에 도달해요.
	tInfo.MonScale = Vec3(80.f, 80.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);

}

CBigEnemyScript::~CBigEnemyScript()
{
}
