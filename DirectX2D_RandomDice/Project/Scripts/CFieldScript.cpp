#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CDiceScript.h"
#include "CRotateScript.h"
#include "CEnemyGateScript.h"



CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
{
	
}

CFieldScript::~CFieldScript()
{
}

#define OBJECT			GetOwner()

void CFieldScript::begin()
{
	CGameObject* pObj = nullptr;
	wstring wstrPath;

	wstrPath = L"texture\\BattleField\\battlefield_normal_bg_top.png";
	OBJECT->GetRenderComponent()->GetDynamicMaterial();
	OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


	// 플레이어 자신일 경우 -> Vec3(0, -210, 1000)
	// 적 플레이어일 경우 -> Vec3(0, -210, 1000)
	//OBJECT->Transform()->SetRelativePos(Vec3(0, -210, 1000));
	OBJECT->Transform()->SetRelativeScale(Vec3(540, 540, 1));

	Vec3 FieldPos = OBJECT->Transform()->GetRelativePos();


	// MeshRender, Transform, alphablend shader, mtrl 달린 프리팹. 기능 없는 오브젝트에 사용 예정
	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	Ptr<CPrefab> pAlphaPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	// MeshRender, Transform, Std2D shader, mtrl 달린 프리팹. 기능 없는 오브젝트에 사용 예정
	wstrPath = L"prefab\\Std2DGameObject.pref";
	Ptr<CPrefab> pStd2DPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	
	wstrPath = L"prefab\\Dice.pref";
	// 주사위 프리팹
	Ptr<CPrefab> pDicePref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
#pragma region _		Set Dice

			// 프리팹 객체화
			CGameObject* pDice = pDicePref->Instantiate();

			// 행렬 위치에 따른 Pos 설정(z : 500)
			Vec3 dicePos = Vec3(((int)i - 2) * 62, ((int)j - 2) * 62 + 120, -500);
			pDice->Transform()->SetRelativePos(dicePos);

			// 스크립트 가져와서 저장
			CDiceScript* DiceScript = m_DiceField[i][j] = pDice->GetScript<CDiceScript>();

			DiceScript->begin();

			// 주사위 스크립트와 쌍방으로 연결
			DiceScript->SetField(this);

			// 주사위 스크립트의 행렬, Pos, Name 설정
			DiceScript->SetDiceXY(i + 1, j + 1);

			// 주사위 종류과 정보 설정 
			DiceScript->SetDiceType(DICE(i + 1));

			// AddObject
			OBJECT->AddChild(pDice);

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







	m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::DEFAULT] = true;
	m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::SPEED] = true;
	m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::BIG] = true;
}

void CFieldScript::tick()
{

	//==================
	// Spawn Enemy Check
	//==================

	Vec3 FieldPos = OBJECT->Transform()->GetRelativePos();

	Vec3 Gate1Pos(m_EnemyGate1->Transform()->GetWorldPos());
	Vec3 Line2Pos(m_Line2->Transform()->GetWorldPos());
	Vec3 Gate2Pos(m_EnemyGate2->Transform()->GetWorldPos());

	float YLineLen = abs(Line2Pos.y - Gate1Pos.y);
	float XLineLen = abs(Gate1Pos.x - Gate2Pos.x);

	ENEMY_PAIR SpawnEnemy{};

	///// DEFAULT
	if (m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::DEFAULT])
	{
		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::DEFAULT]->Instantiate();
		SpawnEnemy.pObject->SetName(L"DefaultEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pEnemyScript->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Gate1Pos.x, Gate1Pos.y, 600));

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::DEFAULT] = false;
	}

	///// SPEED
	if (m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::SPEED])
	{
		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::SPEED]->Instantiate();
		SpawnEnemy.pObject->SetName(L"SpeedEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Gate1Pos.x, Gate1Pos.y, 600));

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::SPEED] = false;
	}

	///// BIG
	if (m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::BIG])
	{
		// 1. 프리팹 객체화
		SpawnEnemy.pObject = m_EnemyPrefab[(UINT)ENEMY_TYPE::BIG]->Instantiate();
		SpawnEnemy.pObject->SetName(L"BigEnemy");

		SpawnEnemy.pEnemyScript = SpawnEnemy.pObject->GetScript<CEnemyScript>();

		// 시작 게이트 Pos
		SpawnEnemy.pObject->begin();
		SpawnEnemy.pObject->Transform()->SetRelativePos(Vec3(Gate1Pos.x, Gate1Pos.y, 600));

		// 2. EnemyList에 삽입
		m_EnemyList.push_back(SpawnEnemy);

		// 3. SpawnObject로 게임에 생성
		GamePlayStatic::SpawnGameObject(SpawnEnemy.pObject, 7);

		m_bSpawnEnemyArr[(UINT)ENEMY_TYPE::BIG] = false;
	}

	
	///////////////////


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

		if (MoveProgress <= 31.f) // 왼 쪽 라인에 위치
		{
			Pos.x = Gate1Pos.x;
			Pos.y = Gate1Pos.y + YLineLen * (MoveProgress / 31.f);
		}
		else if (MoveProgress > 69.f) // 오른 쪽 라인에 위치
		{
			Pos.x = Gate2Pos.x;

			if (MoveProgress > 100.f)
			{
				ThisEnemyIsDead = true;
				Pos.y = Gate2Pos.y;
				Pos.z = OBJECT->Transform()->GetRelativePos().z + 100;
			}
			else
			{
				Pos.y = Line2Pos.y - YLineLen * ((MoveProgress - 69.f) / 31.f);
			}
		}
		else // 중앙 라인에 위치
		{
			Pos.x = Gate1Pos.x + XLineLen * ((MoveProgress - 31.f) / 38.f);
			Pos.y = Line2Pos.y;
		}

		pObject->Transform()->SetRelativePos(Pos);


		//==================
		// Check Enemy Dead
		//==================

		if (!pEScript->IsEndDeathParticle())
		{
			if (ThisEnemyIsDead)
				pEScript->PlayDeathParticle();

			++it;
		}
		// 만약 사망 파티클 출력이 끝났을 경우
		else
		{
			// 레벨에서 제거
			GamePlayStatic::DestroyGameObject(pObject);

			// EnemyList에서 제거
			it = m_EnemyList.erase(it);
		}

	}


}

#undef OBJECT

