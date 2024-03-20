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
	// �ڽ��� ���൵�� ������
	m_MoveProgress += m_EnemyInfo.MoveSpeed * DT * 5;

	// ���߿� ������ ���� �ϴ� �ڵ�/////////////////////////////////////////////////////
	if (m_MoveProgress >= 100.f)
	{
		PlayDeathParticle();
		m_MoveProgress = 0.f;
	}

	// ��� ��ƼŬ ���
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

	// ���� �޽� ���� ������Ʈ�� ���� ���, �޽� ���� ������Ʈ ����
	if (OBJECT->GetRenderComponent() == nullptr)
		OBJECT->AddComponent(new CMeshRender);

	// ���� �޽� ���� ������Ʈ�� �ƴ� �ٸ� ���� ������Ʈ�� ������� ���, assert()
	if (OBJECT->GetRenderComponent()->GetType() != COMPONENT_TYPE::MESHRENDER)
		assert(nullptr);

	OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));

	// �ڽ��� �����ڷκ��� �޾ƿ� �� �ؽ��� ���ε�
	OBJECT->MeshRender()->GetDynamicMaterial();
	OBJECT->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_EnemyInfo.MonTex);
	OBJECT->Transform()->SetRelativeScale(m_EnemyInfo.MonScale);

	// ��ƼŬ ������Ʈ ����
	m_pParticleObject = new CGameObject;
	m_pParticleObject->AddComponent(new CTransform);

	// ��ƼŬ �ý��� ������Ʈ ����
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// ��ƼŬ ������Ʈ�� ��ƼŬ �ý��� ����
	wstring strPath = L"texture\\particle\\smokeparticle.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));

	// ��ƼŬ ������Ʈ �߰�
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
