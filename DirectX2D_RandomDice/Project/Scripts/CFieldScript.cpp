#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CFontMgr.h>
#include <Engine/CDevice.h>

#include "CDiceScript.h"
#include "CDiceUI.h"
#include "CEnemyScript.h"
#include "CEnemyGateScript.h"




CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
	, AutoSpawnEnemy(true)
	, m_SummonSP(10)
	, m_SP(100)
	, m_MaxEnemyHP(0)
	, m_gen(m_rd())
	, m_XDis(0, 4)
	, m_YDis(0, 2)
	, m_CurWave(1)
	, IsInvincible(false)
{

	// 나중에는 여기서 초기화하면 안된다.

	m_Deck[0] = DICE(2);
	m_Deck[1] = DICE(2);
	m_Deck[2] = DICE(2);
	m_Deck[3] = DICE(2);
	m_Deck[4] = DICE(2);
}

CFieldScript::~CFieldScript()
{
}

#define OBJECT			GetOwner()

struct CompareEnemyPair
{
	bool operator()(const ENEMY_PAIR& enemy1, const ENEMY_PAIR& enemy2) const
	{
		// 기준으로 삼을 멤버 변수에 접근하여 비교
		return enemy1.pEnemyScript->GetMoveProgress() < enemy2.pEnemyScript->GetMoveProgress();
	}
};


void CFieldScript::begin()
{
	CDiceScript::InitDice();


	// 예외 처리
	
	assert(OBJECT);

	if (OBJECT->GetRenderComponent() == nullptr)
	{
		OBJECT->AddComponent(new CMeshRender);
		OBJECT->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		OBJECT->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"AlphaBlendMtrl"));
	}

	assert(OBJECT->MeshRender());
	OBJECT->MeshRender()->GetDynamicMaterial();

	CGameObject* pObj = nullptr;
	wstring wstrPath;

	wstrPath = L"texture\\BattleField\\battlefield_normal_bg_top.png";
	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


	// 플레이어 자신일 경우 -> Vec3(0, -210, 1000)
	// 적 플레이어일 경우 -> Vec3(0, 340, 1000)
	//OBJECT->Transform()->SetRelativePos(Vec3(0, -210, 1000));
	OBJECT->Transform()->SetRelativeScale(Vec3(540, 540, 1));

	Vec3 FieldPos = OBJECT->Transform()->GetRelativePos();


	// MeshRender, Transform, alphablend shader, mtrl 달린 프리팹. 기능 없는 오브젝트에 사용 예정
	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	Ptr<CPrefab> pAlphaPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	// MeshRender, Transform, Std2D shader, mtrl 달린 프리팹. 기능 없는 오브젝트에 사용 예정
	wstrPath = L"prefab\\Std2DGameObject.pref";
	Ptr<CPrefab> pStd2DPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	
	// 주사위 프리팹
	wstrPath = L"prefab\\Dice.pref";
	Ptr<CPrefab> pDicePref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
#pragma region _		Set Dice

			// 프리팹 객체화
			CGameObject* pDice = pDicePref->Instantiate();
			OBJECT->AddChild(pDice);

			// 행렬 위치에 따른 Pos 설정(z : 500)
			Vec3 dicePos = Vec3(((float)i - 2) * 62, ((float)j - 2) * 62 + 120, -500);
			pDice->Transform()->SetRelativePos(dicePos);

			// 스크립트 가져와서 저장
			CDiceScript* DiceScript = m_DiceField[i][j] = pDice->GetScript<CDiceScript>();

			// 주사위 스크립트와 쌍방으로 연결
			DiceScript->SetField(this);

			// 주사위 스크립트의 행렬, Pos, Name 설정
			DiceScript->SetDiceXY(i + 1, j + 1);

			// 주사위 종류과 정보 설정 
			DiceScript->SetDiceType(DICE::NONE);

			// AddObject

#pragma endregion

#pragma region _		Background Object

			//////////////////////////////////
			// battlefield_normal_dicebg_1 ~ 3
			//////////////////////////////////
	
			pObj = pAlphaPref->Instantiate();
			
			// 주사위 pos 값 그대로 사용
			pObj->Transform()->SetRelativePos(Vec3(dicePos.x, dicePos.y, -20));
			pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
			pObj->Transform()->SetAbsolute(true);

			wstrPath = L"texture\\BattleField\\Dice\\battlefield_normal_dicebg_" + to_wstring((i + j) % 3 + 1) + L".png";
			pObj->SetName(wstrPath);

			pObj->MeshRender()->GetDynamicMaterial();
			pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


			OBJECT->AddChild(pObj);

		}
	}

	////////////////////////////////
	// battlefield_normal_bottom_bg
	////////////////////////////////
	
	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deck\\battlefield_normal_bottom_bg.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -215.f, -30));
	pObj->Transform()->SetRelativeScale(Vec3(510.f, 110.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);


	////////////////////////////////
	// battlefield_normal_vs_line_1
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-230.f, 50.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(33.f, 340.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_vs_line_2
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-10.f, 213.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(407.f, 19.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
	
	m_Line2 = pObj;

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_vs_line_3
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_3.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(216.f, 50.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(46.f, 343.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_1
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(180.f, -70.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(75.f, 75.f, 1.f));
	pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, XM_PI / 3));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_2
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(189.f, 109.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 40.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////////////////
	// battlefield_normal_deco_4
	////////////////////////////////

	pObj = pAlphaPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_4.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-190.f, 165.f, -10.f));
	pObj->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
	pObj->Transform()->SetAbsolute(true);

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	OBJECT->AddChild(pObj);

	////////////////////
	// ingame_mob_gate 1
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(-235.f, -110.f, -20));
	pObj->Transform()->SetAbsolute(true);

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_1");
	
	m_EnemyGate1 = pObj;

	OBJECT->AddChild(pObj);

	////////////////////
	// ingame_mob_gate 2
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(230.f, -110.f, -20));
	pObj->Transform()->SetAbsolute(true);

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_2");
	
	m_EnemyGate2 = pObj;

	OBJECT->AddChild(pObj);

#pragma endregion


	//==================
	// EnemyPrefab 세팅
	//==================
	
	wstrPath = L"prefab\\DefaultEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);
	
	wstrPath = L"prefab\\SpeedEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);
	
	wstrPath = L"prefab\\BigEnemy.pref";
	m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG] = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT].Get());
	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED].Get());
	assert(m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG].Get());






	// 초기 스폰 타이밍 조절
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] = 4.f;
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] = 7.f;
	m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] = 13.f;



	//===================
	// CDiceUI 덱 정보 세팅
	//===================

	for (UINT i = 0; i < 5; ++i)
	{
		// 새 자식 오브젝트 생성
		pObj = new CGameObject;
		OBJECT->AddChild(pObj);

		// DiceUI 추가
		CDiceUI* pDiceUI = new CDiceUI;
		pObj->AddComponent(pDiceUI);
		pObj->SetName(L"DiceUI_" + to_wstring(i));

		// begin에서 필요한 정보 세팅
		pDiceUI->SetDice(m_Deck[i]);
		pDiceUI->SetDeckIdx(i);
		pDiceUI->SetField(this);
	}







	//==================
	// Debug
	//==================
	AddScriptParam(SCRIPT_PARAM::INT, "Cur Wave", &m_CurWave);
	AddScriptParam(SCRIPT_PARAM::INT, "Cur SP", &m_SP);
	AddScriptParam(SCRIPT_PARAM::INT, "DEFAULT Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount);
	AddScriptParam(SCRIPT_PARAM::INT, "BIG Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount);
	AddScriptParam(SCRIPT_PARAM::INT, "SPEED Enemy Spawn Count", &m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount);
	AddScriptParam(SCRIPT_PARAM::INT, "Enemy Auto Spawn", &AutoSpawnEnemy);
	AddScriptParam(SCRIPT_PARAM::INT, "IsInvincible", &IsInvincible);


}

void CFieldScript::tick()
{

	//==========================
	// AttackPriority 배열 Clear
	//==========================
	for (UINT i = 0; i < (UINT)ATTACK_PRIORITY::END; ++i)
		m_AttackPriority[i] = ENEMY_PAIR();


	for (UINT i = 0; i < (UINT)ENEMY_TYPE::END; ++i)
	{
		// Enemy Auto Spawn Timer
		m_AccSpawnCoolDown[i] -= m_EnemySpawnRate[m_CurWave] * DT;

		// Enemy Spawn 간격
		//	- 적이 겹쳐 소환되는걸 방지함.
		m_SpawnEnemyCheck[i].CoolDown -= DT;

		//  Enemy Spawn 간격이 돌았으면 0으로 초기화 해놓기
		if (m_SpawnEnemyCheck[i].CoolDown < 0.f)
			m_SpawnEnemyCheck[i].CoolDown = 0.f;
	}

	//======================
	// Enemy Auto Spawn
	//======================
	//	- 스폰 개수를 증가시켜서 이후에 라인에 적 오브젝트를 추가하도록 함.
	if (AutoSpawnEnemy) // 디버그용 bool 값.
	{
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::DEFAULT] = 2.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount;
		}
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::SPEED] = 7.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount;
		}
		if (m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] < 0.f)
		{
			m_AccSpawnCoolDown[(UINT)ENEMY_TYPE::BIG] = 10.f;
			++m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount;
		}
	}


	//======================
	// Update MaxEnemyHealth
	//======================
	m_EnemyHPUpdateTimer += DT;

	if (m_EnemyHPUpdateTimer > 5.f)
	{
		// 5초마다 적의 체력을 현재 라운드에 비례해서 증가시킴.
		m_MaxEnemyHP += m_EnemyHPArr[m_CurWave];
		m_EnemyHPUpdateTimer = 0.f;
	}


	//===============
	// Spawn Enemy
	//===============
	Vec3 Line1StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_EnemyGate1->Transform()->GetWorldPos().y, 600);
	Vec3 Line2StartPos(m_EnemyGate1->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 Line3StartPos(m_EnemyGate2->Transform()->GetWorldPos().x, m_Line2->Transform()->GetWorldPos().y, 600);
	Vec3 LineEndPos	  (m_EnemyGate2->Transform()->GetWorldPos().x, m_EnemyGate2->Transform()->GetWorldPos().y, 600);

	float YLineLen = Line2StartPos.y - Line1StartPos.y;
	float XLineLen = Line3StartPos.x - Line2StartPos.x;

	ENEMY_PAIR SpawnEnemy{};

	///// DEFAULT
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT]->Instantiate();
		SpawnEnemy.pObject->SetName(L"DefaultEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pEnemyScript->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::DEFAULT].EnemySpawnCount;
	}

	///// SPEED
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED]->Instantiate();
		SpawnEnemy.pObject->SetName(L"SpeedEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Line1StartPos);

		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP);
		SpawnEnemy.pEnemyScript->SetField(this);

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::SPEED].EnemySpawnCount;
	}

	///// BIG
	if (m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount > 0
		&& m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnableSpawn())
	{

		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG]->Instantiate();
		SpawnEnemy.pObject->SetName(L"BigEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Line1StartPos.x, Line1StartPos.y, 599.f));
		
		// 체력 설정
		SpawnEnemy.pEnemyScript->SetEnemyHealth(m_MaxEnemyHP * 5);
		SpawnEnemy.pEnemyScript->SetField(this);
		
		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);


		m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].CoolDown = 0.5f;
		--m_SpawnEnemyCheck[(UINT)ENEMY_TYPE::BIG].EnemySpawnCount;
	}

	
	///////////////////
	///////////////////
	///////////////////


	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	for (auto it = m_EnemyList.begin(); it != m_EnemyList.end(); )
	{
		ENEMY_PAIR pEnemy = *it;

		CGameObject* pObject = pEnemy.pObject;
		CEnemyScript* pEScript = pEnemy.pEnemyScript;

		bool ThisEnemyIsDead = false;

		//==================
		// Set Enemy Pos
		//==================

		// 위치 비율 - 31 : 38 : 31
		float MoveProgress = pEScript->GetMoveProgress();

		Vec3 Pos(0.f, 0.f, 600.f);

		// 라인에 Enemy Pos Set
		if (MoveProgress <= 31.f) // 왼 쪽 라인에 위치
		{
			Pos = Line1StartPos;
			Pos.y += YLineLen * (MoveProgress / 31.f);
		}
		else if (MoveProgress > 69.f) // 오른 쪽 라인에 위치
		{
			if (MoveProgress > 100.f)
			{
				Pos = LineEndPos;
				ThisEnemyIsDead = true;
			}
			else
			{
				Pos = Line3StartPos;
				Pos.y -= YLineLen * ((MoveProgress - 69.f) / 31.f);
			}
		}
		else // 중앙 라인에 위치
		{
			Pos = Line2StartPos;
			Pos.x += XLineLen * ((MoveProgress - 31.f) / 38.f);
		}

		if (pEScript->GetEnemyType() == ENEMY_TYPE::BIG)
			Pos.z += 2;

		if (pEScript->GetEnemyType() == ENEMY_TYPE::SPEED)
			Pos.z -= 2;

		pObject->Transform()->SetRelativePos(Pos);


		//==================
		// Check Enemy Dead
		//==================

		// DamageCheck
		if (pEScript->GetEnemyHealth() <= 0)
			ThisEnemyIsDead = true;

		if (ThisEnemyIsDead)
		{
			Pos.z = Transform()->GetWorldPos().z + 100;
			pObject->Transform()->SetRelativePos(Pos);
			pEScript->SetDeadEnemy();

			// EnemyList에서 제거
			it = m_EnemyList.erase(it);
		}
		else
		{
			++it;
		}
	}

	m_EnemyList.sort(CompareEnemyPair());

	//====================
	// Attack Priority Set
	//====================

	if (!m_EnemyList.empty())
	{
		// ATTACK_PRIORITY::FRONT
		m_AttackPriority[(UINT)ATTACK_PRIORITY::FRONT] = m_EnemyList.back();

		// ATTACK_PRIORITY::HIGH_HEALTH
		m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH] = m_EnemyList.front();
		for (auto& it : m_EnemyList)
		{
			if (it.pEnemyScript->GetEnemyHealth() > m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH].pEnemyScript->GetEnemyHealth())
				m_AttackPriority[(UINT)ATTACK_PRIORITY::HIGH_HEALTH] = it;
		}
	}


}


UINT CFieldScript::DiceLevelUp(DICE _dice, UINT _idx)
{
	if (m_DiceLevel[_idx] == 5)
		return 0;

	if (m_LevelUpSP[m_DiceLevel[_idx] - 1] > m_SP)
		return m_LevelUpSP[m_DiceLevel[_idx] - 1];

	// 모든 주사위를 순회하면서 _dice와 같은 유형의 주사위를 싹 가져오기
	for (UINT i = 0; i < 5; ++i)
	{
		for (UINT j = 0; j < 3; ++j)
		{
			CDiceScript* pDice = m_DiceField[i][j];

			if (pDice->GetDice() == _dice)
			{
				// 해당 주사위들에게 레벨업 함수 호출
				pDice->PlayLevelUp();
			}
		}
	}

	// 주사위 레벨업
	m_SP -= m_LevelUpSP[m_DiceLevel[_idx] - 1];

	// 올라간 비용 인덱스 반환
	return m_LevelUpSP[++m_DiceLevel[_idx] - 1];
}

UINT CFieldScript::GetCurDiceLevel(DICE _dice) const
{
	int i = 0;
	for (; i <= 5; ++i)
	{
		if (i == 5)
			return 0;
		if (m_Deck[i] == _dice)
			return m_DiceLevel[i];
	}

	return 0;
}

void CFieldScript::SummonDice()
{ 
	// 필드가 모두 차 있거나, 보유 SP가 소환 SP보다 적을 경우 return
	if (m_CurDiceCount >= 15 || m_SP < m_SummonSP)
		return;

	// SP 소모하기
	m_SP -= m_SummonSP;

	// 소환 SP 증가
	m_SummonSP += 10;


	CDiceScript* pDice = nullptr;

	// 랜덤한 빈 필드 공간 찾기
	while (true)
	{
		pDice = GetRandomDice();
		if (pDice != nullptr && pDice->GetDice() == DICE::NONE)
			break;
	}

	// 나중에는 덱 중에 골라서 넣어주어야 한다.
	pDice->SetDiceType(m_Deck[m_XDis(m_gen)]);

	++m_CurDiceCount;
}

void CFieldScript::SpawnDice(UINT _LOW, UINT _COL)
{
}

#undef OBJECT

