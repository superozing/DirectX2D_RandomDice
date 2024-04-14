#include "pch.h"
#include "CSnakeBoss.h"

#include "CFieldScript.h"

CSnakeBoss::CSnakeBoss()
	: CBossScript(SNAKEBOSS)
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BOSS);

	tParticleModule tModule;

	// �ʱ� ��� ����		
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
	tModule.VelocityAlignment = 1; // �ӵ��� ���� ���� ����
	tModule.AlphaBasedLife = 0; // 0 : off, 1 : NomrlizedAge, 2: Age
	tModule.AlphaMaxAge = 2.f;

	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	tModule.DragTime = 1.f;

	ENEMY_INFO tInfo;

	tInfo.DeathParticleModule = tModule;

	tInfo.MonTex = TEX_LOAD(L"texture\\Enemy\\Boss\\boss_1.png");
	tInfo.MoveSpeed = 5.f; // �ʴ� ���൵ 5 ����. �� 20�ʰ� �ɷ��� �� ������ �����ؿ�.
	tInfo.MonScale = Vec3(100.f, 100.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);

}

CSnakeBoss::CSnakeBoss(const CSnakeBoss& _Origin)
	: CBossScript((SCRIPT_TYPE)_Origin.GetScriptType())
{
	CEnemyScript::SetEnemyType(ENEMY_TYPE::BOSS);

	tParticleModule tModule;

	// �ʱ� ��� ����		
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
	tModule.VelocityAlignment = 1; // �ӵ��� ���� ���� ����
	tModule.AlphaBasedLife = 0; // 0 : off, 1 : NomrlizedAge, 2: Age
	tModule.AlphaMaxAge = 2.f;

	tModule.arrModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = 1;
	tModule.DragTime = 1.f;

	ENEMY_INFO tInfo;

	tInfo.DeathParticleModule = tModule;

	tInfo.MonTex = TEX_LOAD(L"texture\\Enemy\\Boss\\boss_1.png");
	tInfo.MoveSpeed = 5.f; // �ʴ� ���൵ 5 ����. �� 20�ʰ� �ɷ��� �� ������ �����ؿ�.
	tInfo.MonScale = Vec3(100.f, 100.f, 1.f);
	CEnemyScript::SetEnemyInfo(tInfo);
}

CSnakeBoss::~CSnakeBoss()
{
}

void CSnakeBoss::BossSkill()
{
	m_IsActivateSkill = true;
	m_SkillTimer = 0.f;
	m_SkillDelayTimer = 1.f;
	
	CFieldScript* field = GetField();

	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::SPEED);
	field->SpawnEnemy(ENEMY_TYPE::BIG);

	// ��ų ���� ������ ��ŭ �����ֱ�.

	SetMoveSpeed(0.f);


	// �ɷ��� �̰� �´µ�,
	// ��ų�� ����� �� ������ ���� ȿ���� �־��־�� �Ѵ�.

}

void CSnakeBoss::EndBossSkill()
{
	m_IsActivateSkill = false;
	m_SkillDelayTimer = 0.f;
	m_SkillTimer = 10.f;

	CFieldScript* field = GetField();

	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::DEFAULT);
	field->SpawnEnemy(ENEMY_TYPE::SPEED);
	field->SpawnEnemy(ENEMY_TYPE::BIG);

	// ��ų ���� ������ ��ŭ �����ֱ�.

	SetMoveSpeed(5.f);

	// �ɷ��� �̰� �´µ�,
	// ��ų�� ����� �� ������ ���� ȿ���� �־��־�� �Ѵ�.

}

void CSnakeBoss::begin()
{
	CEnemyScript::begin();
	m_SkillTimer = 2.f;
}

void CSnakeBoss::tick()
{
	CEnemyScript::tick();

	if (!m_IsActivateSkill)
	{
		m_SkillTimer -= DT;

		if (m_SkillTimer < 0.f)
			BossSkill();
	}
	else
	{
		m_SkillDelayTimer -= DT;

		if (m_SkillDelayTimer < 0.f)
			EndBossSkill();
	}
}
