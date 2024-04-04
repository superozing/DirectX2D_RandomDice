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
	// 자신의 진행도를 갱신함
	m_MoveProgress += m_EnemyInfo.MoveSpeed * DT;

	if (m_fScale > 1.05f)
		m_Minus = -1;
	if (m_fScale < 0.95f)
		m_Minus = 1;

	m_fScale += m_Minus * 0.15f * DT;

	// 사망 파티클 출력
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
		// 체력 폰트의 위치 설정
		m_fInfo1.fPosX = Pos.x;
		m_fInfo1.fPosY = Pos.y;
		m_fInfo2.fPosX = Pos.x;
		m_fInfo2.fPosY = Pos.y;

		m_fInfo1.WorldRenderOffset.y = 7;
		m_fInfo2.WorldRenderOffset.y = 10;

		// 체력 폰트의 문자열 설정
		UINT enemyHP = GetEnemyHealth();

		// 단위 맟추기
		if (enemyHP < 10000)
		{
			m_fInfo1.WStr = to_wstring(enemyHP);
			m_fInfo2.WStr = to_wstring(enemyHP);
		}
		else if (enemyHP < 1000000) // enemyHP 가 10000보다 커질 경우 가독성을 위해 1000을 나누어 K로 표시
		{
			enemyHP /= 1000;
			m_fInfo1.WStr = to_wstring(enemyHP) + L"K";
			m_fInfo2.WStr = to_wstring(enemyHP) + L"K";
		}
		else // enemyHP 가 1000000보다 커질 경우 가독성을 위해 1000000을 나누어 M로 표시
		{
			enemyHP /= 1000000;
			m_fInfo1.WStr = to_wstring(enemyHP) + L"M";
			m_fInfo2.WStr = to_wstring(enemyHP) + L"M";
		}

		// 폰트 매니저 출력에 추가
		CFontMgr::GetInst()->AddRenderFont(m_fInfo2);
		CFontMgr::GetInst()->AddRenderFont(m_fInfo1);
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

	// vScale 세팅
	m_vScale = m_EnemyInfo.MonScale;

	// 파티클 오브젝트 생성
	m_pParticleObject = new CGameObject;
	m_pParticleObject->AddComponent(new CTransform);
	m_pParticleObject->SetName(L"DeathParticleObject");

	// 파티클 시스템 컴포넌트 부착
	m_ParticleSystem = new CParticleSystem;
	m_pParticleObject->AddComponent(m_ParticleSystem);

	// 파티클 오브젝트의 파티클 시스템 세팅
	wstring strPath = L"texture\\particle\\cloud4.png";
	m_ParticleSystem->SetParticleModule(m_EnemyInfo.DeathParticleModule);
	m_ParticleSystem->SetParticleTexture(CAssetMgr::GetInst()->Load<CTexture>(strPath, strPath));

	// 파티클 출력 위치
	m_pParticleObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -650.f));

	// 파티클 오브젝트 추가
	OBJECT->AddChild(m_pParticleObject);


	SetEnemyHealth(100);

	//==================
	// FONTINFO 세팅 
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

	// m_DeathParticleTimer의 초기 값은 -1이다.
	m_DeathParticleTimer = 0.f;

	int AddSP = 10 * m_OwnerField->GetCurWave();

	// 체력이 전부 떨어져서 죽었는가? 아니면 라인의 끝에 도달해서 죽었는가?
	if (m_CurHealth > 0)
		m_OwnerField->SetPlayerHP(m_OwnerField->GetPlayerHP() - 1);
		
	if (m_EnemyType == ENEMY_TYPE::BIG)
	{
		AddSP *= 5;

		if (m_CurHealth > 0)
			m_OwnerField->SetPlayerHP(m_OwnerField->GetPlayerHP() - 1); // 빅 몬스터일 경우, 체력 2개 빼주기
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
