#include "pch.h"
#include "CEnemyScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CEnemyScript::CEnemyScript()
	: CScript(ENEMYSCRIPT)
	, m_DeathParticleTimer(-1.f)
{
}

CEnemyScript::CEnemyScript(const CEnemyScript& _Origin)
	: CScript(ENEMYSCRIPT)
	, m_DeathParticleTimer(-1.f)
{
}

CEnemyScript::~CEnemyScript()
{
}

#define OBJECT GetOwner()

void CEnemyScript::tick()
{
	// 자신의 진행도를 갱신함
	m_MoveProgress += m_EnemyInfo.MoveSpeed * DT * 5;

	// 나중에 밖으로 뺴야 하는 코드/////////////////////////////////////////////////////
	if (m_MoveProgress >= 100.f)
	{
		PlayDeathParticle();
		m_MoveProgress = 0.f;
	}

	// 사망 파티클 출력
	if (m_ParticleSystem != nullptr && m_DeathParticleTimer != -1.f)
	{
		m_DeathParticleTimer += DT;
		m_ParticleSystem->SetActivate(true);
		if (m_DeathParticleTimer > 0.1f)
		{
			m_ParticleSystem->SetActivate(false);
			m_DeathParticleTimer = -1.f;
		}
	}
}

void CEnemyScript::begin()
{
	assert(OBJECT);

	// 만약 메쉬 렌더 컴포넌트가 없을 경우, 메쉬 렌더 컴포넌트 생성
	if (OBJECT->GetRenderComponent() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	// 만약 메쉬 렌더 컴포넌트가 아닌 다른 렌더 컴포넌트가 들어있을 경우, assert()
	if (OBJECT->GetRenderComponent()->GetType() != COMPONENT_TYPE::MESHRENDER)
		assert(nullptr);

	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));

	// 자식의 생성자로부터 받아온 적 텍스쳐 바인딩
	OBJECT->MeshRender()->GetDynamicMaterial();
	OBJECT->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_EnemyInfo.MonTex);
	OBJECT->Transform()->SetRelativeScale(m_EnemyInfo.MonScale);

	// 파티클 오브젝트 생성
	m_pParticleObject = new CGameObject;
	m_pParticleObject->AddComponent(new CTransform);

	// 파티클 시스템 컴포넌트 부착
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// 파티클 오브젝트의 파티클 시스템 세팅
	wstring strPath = L"texture\\particle\\smokeparticle.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));

	// 파티클 오브젝트 추가
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_pParticleObject, L"Particle");
}

void CEnemyScript::PlayDeathParticle()
{
	if (m_DeathParticleTimer == -1.f)
		m_DeathParticleTimer = 0.f;
	auto pos = OBJECT->Transform()->GetRelativePos();
	
	m_pParticleObject->Transform()->SetRelativePos(Vec3(pos.x, pos.y, pos.z - 1));
}

void CEnemyScript::SetEnemyType(ENEMY_TYPE _Enemytype)
{
	m_EnemyType = _Enemytype;
}

void CEnemyScript::SetEnemyInfo(ENEMY_INFO _EnemyInfo)
{
	m_EnemyInfo = _EnemyInfo;
}
