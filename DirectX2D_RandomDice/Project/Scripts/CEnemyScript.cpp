#include "pch.h"
#include "CEnemyScript.h"
#include "CFieldScript.h"
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

	if (m_fScale > 1.05f)
		m_Minus = -1;
	if (m_fScale < 0.95f)
		m_Minus = 1;

	m_fScale += m_Minus * 0.15f * DT;

	// ��� ��ƼŬ ���
	if (m_ParticleSystem != nullptr && m_DeathParticleTimer != -1.f)
	{
		if (m_DeathParticleTimer > 2.f)
		{
			EndDeathParticle = true;
			GamePlayStatic::DestroyGameObject(GetOwner());
		}

		m_DeathParticleTimer += DT;
		m_ParticleSystem->SetActivate(true);

		if (m_DeathParticleTimer > 0.1f)
			m_ParticleSystem->SetActivate(false);
	}

	OBJECT->Transform()->SetRelativeScale(Vec3(m_vScale.x, m_vScale.y * m_fScale, m_vScale.z));


	if (!m_IsDeadEnemy)
	{
		//=========
		// HP Text
		//=========
		Vec3 Pos = Transform()->GetWorldPos();
		// ü�� ��Ʈ�� ��ġ ����
		m_fInfo1.fPosX = Pos.x;
		m_fInfo1.fPosY = Pos.y;
		m_fInfo2.fPosX = Pos.x;
		m_fInfo2.fPosY = Pos.y;

		m_fInfo1.WorldRenderOffset.y = 7;
		m_fInfo2.WorldRenderOffset.y = 10;

		// ü�� ��Ʈ�� ���ڿ� ����
		UINT enemyHP = GetEnemyHealth();

		// ���� ���߱�
		if (enemyHP < 10000)
		{
			m_fInfo1.WStr = to_wstring(enemyHP);
			m_fInfo2.WStr = to_wstring(enemyHP);
		}
		else if (enemyHP < 1000000) // enemyHP �� 10000���� Ŀ�� ��� �������� ���� 1000�� ������ K�� ǥ��
		{
			enemyHP /= 1000;
			m_fInfo1.WStr = to_wstring(enemyHP) + L"K";
			m_fInfo2.WStr = to_wstring(enemyHP) + L"K";
		}
		else // enemyHP �� 1000000���� Ŀ�� ��� �������� ���� 1000000�� ������ M�� ǥ��
		{
			enemyHP /= 1000000;
			m_fInfo1.WStr = to_wstring(enemyHP) + L"M";
			m_fInfo2.WStr = to_wstring(enemyHP) + L"M";
		}

		// ��Ʈ �Ŵ��� ��¿� �߰�
		CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
		CFontMgr::GetInst()->AddRenderFont(m_fInfo1);
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

	// vScale ����
	m_vScale = m_EnemyInfo.MonScale;

	// ��ƼŬ ������Ʈ ����
	m_pParticleObject = new CGameObject;
	m_pParticleObject->AddComponent(new CTransform);
	m_pParticleObject->SetName(L"DeathParticleObject");

	// ��ƼŬ �ý��� ������Ʈ ����
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// ��ƼŬ ������Ʈ�� ��ƼŬ �ý��� ����
	wstring strPath = L"texture\\particle\\cloud4.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));

	// ��ƼŬ ��� ��ġ
	m_pParticleObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -650.f));

	// ��ƼŬ ������Ʈ �߰�
	OBJECT->AddChild(m_pParticleObject);


	SetEnemyHealth(100);

	//==================
	// FONTINFO ���� 
	//==================
	m_fInfo1.Color = FONT_RGBA(0, 0, 0, 255);
	m_fInfo1.fFontSize = 15.f;
	m_fInfo1.FontType = FONT_TYPE::ALBA_SUPER;
	m_fInfo1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fInfo1.IsWorldPosRender = true;

	m_fInfo2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fInfo2.fFontSize = 15.2f;
	m_fInfo2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fInfo2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fInfo2.IsWorldPosRender = true;
}

void CEnemyScript::SetDeadEnemy()
{
	m_IsDeadEnemy = true;

	// m_DeathParticleTimer�� �ʱ� ���� -1�̴�.
	m_DeathParticleTimer = 0.f;

	int AddSP = 10 * m_OwnerField->GetCurWave();

	// ü���� ���� �������� �׾��°�? �ƴϸ� ������ ���� �����ؼ� �׾��°�?
	if (m_CurHealth > 0)
		m_OwnerField->SetPlayerHP(m_OwnerField->GetPlayerHP() - 1);
		
	if (m_EnemyType == ENEMY_TYPE::BIG)
	{
		AddSP *= 5;

		if (m_CurHealth > 0)
			m_OwnerField->SetPlayerHP(m_OwnerField->GetPlayerHP() - 1); // �� ������ ���, ü�� 2�� ���ֱ�
	}

	m_OwnerField->AddCurSP(AddSP);
}

void CEnemyScript::SetEnemyType(ENEMY_TYPE _Enemytype)
{
	m_EnemyType = _Enemytype;
}

void CEnemyScript::SetEnemyInfo(ENEMY_INFO _EnemyInfo)
{
	m_EnemyInfo = _EnemyInfo;
}
