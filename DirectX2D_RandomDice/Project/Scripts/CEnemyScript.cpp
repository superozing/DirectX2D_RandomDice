#include "pch.h"
#include "CEnemyScript.h"

CEnemyScript::CEnemyScript()
	:CScript(ENEMYSCRIPT)
{
}

CEnemyScript::CEnemyScript(const CEnemyScript& _Origin)
	:CScript(ENEMYSCRIPT)
{
}

CEnemyScript::~CEnemyScript()
{
}

#define OBJECT GetOwner()

void CEnemyScript::tick()
{
	static float AccTime = 0.f;

	// 자신의 진행도를 갱신함
	m_MoveProgress += m_EnemyInfo.MoveSpeed * DT;
	AccTime += DT;

	if (m_ParticleSystem != nullptr && AccTime > 0.5f)
	{
		AccTime = 0.f;
		if (!m_ParticleSystem->GetActivate()) AccTime += 0.4f;

		m_ParticleSystem->SetActivate(!m_ParticleSystem->GetActivate());
	}


}

void CEnemyScript::begin()
{
	assert(OBJECT);

	if (OBJECT->GetRenderComponent() == nullptr)
	{
		m_ParticleSystem = new CParticleSystem;
		OBJECT->AddComponent(m_ParticleSystem);
	}
		
	wstring strPath = L"texture\\particle\\smokeparticle.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));
}

void CEnemyScript::SetEnemyType(ENEMY_TYPE _Enemytype)
{
	m_EnemyType = _Enemytype;
}

void CEnemyScript::SetEnemyInfo(ENEMY_INFO _EnemyInfo)
{
	m_EnemyInfo = _EnemyInfo;
}
