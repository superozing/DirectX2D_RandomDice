#include "pch.h"
#include "CEnemyScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CEnemyScript::CEnemyScript()
	: CScript(ENEMYSCRIPT)
	, m_DeathParticleTimer(-1.f)
{
}

CEnemyScript::CEnemyScript(SCRIPT_TYPE _ScriptType)
	:CScript(_ScriptType)
	, m_DeathParticleTimer(-1.f)
{
}

CEnemyScript::CEnemyScript(const CEnemyScript& _Origin)
	: CScript(_Origin.GetScriptType())
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
	m_MoveProgress += m_EnemyInfo.MoveSpeed * DT;

	// ��� ��ƼŬ ���
	if (m_ParticleSystem != nullptr && m_DeathParticleTimer != -1.f)
	{
		if (m_DeathParticleTimer > 2.f)
			EndDeathParticle = true;

		m_DeathParticleTimer += DT;
		m_ParticleSystem->SetActivate(true);

		if (m_DeathParticleTimer > 0.1f)
		{
			m_ParticleSystem->SetActivate(false);
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
	m_pParticleObject->SetName(L"DeathParticleObject");

	// ��ƼŬ �ý��� ������Ʈ ����
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// ��ƼŬ ������Ʈ�� ��ƼŬ �ý��� ����
	wstring strPath = L"texture\\particle\\smokeparticle.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));

	// ��ƼŬ ��� ��ġ
	m_pParticleObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -650.f));

	// ��ƼŬ ������Ʈ �߰�
	OBJECT->AddChild(m_pParticleObject);


	SetEnemyHealth(100);
}

void CEnemyScript::PlayDeathParticle()
{
	//OBJECT->Transform()->SetRelativeScale(Vec3(0, 0, 0));

	if (m_DeathParticleTimer == -1.f)
		m_DeathParticleTimer = 0.f;
}

void CEnemyScript::SetEnemyType(ENEMY_TYPE _Enemytype)
{
	m_EnemyType = _Enemytype;
}

void CEnemyScript::SetEnemyInfo(ENEMY_INFO _EnemyInfo)
{
	m_EnemyInfo = _EnemyInfo;
}
