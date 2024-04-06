#include "pch.h"
#include "CEnemyScript.h"
#include "CFieldScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

// ����
Ptr<CPrefab> CEnemyScript::m_01_firePrefab;
Ptr<CPrefab> CEnemyScript::m_02_electricPrefab;


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

	if (m_fSizeScale > 1.05f)
		m_Minus = -1;
	if (m_fSizeScale < 0.95f)
		m_Minus = 1;

	m_fSizeScale += m_Minus * 0.15f * DT;

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

	OBJECT->Transform()->SetRelativeScale(Vec3(m_vScale.x, m_vScale.y * m_fSizeScale, m_vScale.z));


	if (!m_IsDeadEnemy && Transform()->GetWorldPos().y != 0.f)
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

		if (m_IsDamageFontRender)
		{
			m_DamageFontTimer -= DT;
			if (m_DamageFontTimer < 0)
			{
				m_DamageFontTimer = 1.f;
				m_IsDamageFontRender = false;
			}

			m_DamageFontSizeScale -= DT * 3;
			if (m_DamageFontSizeScale < 1.f)
				m_DamageFontSizeScale = 1.f;

			m_fDamage1.fFontSize = m_SourceFontSize * m_DamageFontSizeScale;
			m_fDamage2.fFontSize = m_SourceFontSize * m_DamageFontSizeScale;

			m_fDamage1.fPosX = Pos.x;
			m_fDamage1.fPosY = Pos.y;
			m_fDamage2.fPosX = Pos.x;
			m_fDamage2.fPosY = Pos.y;

			m_fDamage1.WorldRenderOffset.y = -48;
			m_fDamage2.WorldRenderOffset.y = -45;

			CFontMgr::GetInst()->AddRenderFont(m_fDamage2);
			CFontMgr::GetInst()->AddRenderFont(m_fDamage1);
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

	m_fDamage1.Color = FONT_RGBA(0, 0, 0, 255);
	m_fDamage1.FontType = FONT_TYPE::ALBA_SUPER;
	m_fDamage1.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fDamage1.IsWorldPosRender = true;

	m_fDamage2.Color = FONT_RGBA(255, 255, 255, 255);
	m_fDamage2.FontType = FONT_TYPE::ALBA_MATTER;
	m_fDamage2.TextFlag = FW1_TEXT_FLAG::FW1_CENTER;
	m_fDamage2.IsWorldPosRender = true;
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

void CEnemyScript::TakeDamage(int _Dmg, bool _IsCriticalAttack)
{
	// ������ ������
	m_CurHealth -= _Dmg;

	m_IsDamageFontRender = true;
	m_DamageFontSizeScale = 1.3f;
	m_DamageFontTimer = 1.f;

	if (_IsCriticalAttack)
	{
		m_fDamage2.Color = FONT_RGBA(255, 127, 0, 255);
		m_DamageFontSizeScale = 1.6f;
	}
	else
	{
		m_fDamage2.Color = FONT_RGBA(210, 210, 210, 255);
		m_DamageFontSizeScale = 1.4f;
	}

	m_fDamage1.WStr = to_wstring(_Dmg);
	m_fDamage2.WStr = to_wstring(_Dmg);
}

void CEnemyScript::SetEnemyType(ENEMY_TYPE _Enemytype)
{
	m_EnemyType = _Enemytype;
}

void CEnemyScript::SetEnemyInfo(ENEMY_INFO _EnemyInfo)
{
	m_EnemyInfo = _EnemyInfo;
}


// ������ ������ �����ϴ� �ڵ�
//CGameObject* CEnemyScript::Get01_fireObject()
//{
//	if (m_01_firePrefab == nullptr)
//	{
//		auto pObj = new CGameObject;
//		pObj->SetName(L"01_fireObject");
//		
//		// Transform
//		pObj->AddComponent(new CTransform);
//
//		// MeshRender
//		pObj->AddComponent(new CMeshRender);
//		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
//		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
//		
//		// Animator2D
//		pObj->AddComponent(new CAnimator2D);
//		pObj->Animator2D()->LoadAnimation(L"animdata\\01_fire.txt");
//
//		Ptr<CPrefab> pMissilePrefab = new CPrefab(pObj, true);
//		CAssetMgr::GetInst()->AddAsset<CPrefab>(L"01_firePrefab", pMissilePrefab.Get());
//
//		pMissilePrefab->Save(L"prefab\\01_fire.pref");
//	}
//	return nullptr;
//}

CGameObject* CEnemyScript::Get01_fireObject()
{
	// ù ���� �� ������ �ε�
	if (m_01_firePrefab == nullptr)
	{
		m_01_firePrefab = CAssetMgr::GetInst()->Load<CPrefab>(
			L"prefab\\01_fire.pref", L"prefab\\01_fire.pref");
	}

	return m_01_firePrefab->Instantiate();
}

CGameObject* CEnemyScript::Get02_electricObject()
{
	// ù ���� �� ������ �ε�
	if (m_02_electricPrefab == nullptr)
	{
		m_02_electricPrefab = CAssetMgr::GetInst()->Load<CPrefab>(
			L"prefab\\02_electric.pref", L"prefab\\02_electric.pref");
	}

	return m_02_electricPrefab->Instantiate();
}
