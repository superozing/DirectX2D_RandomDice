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


#define OBJECT			GetOwner()


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
			DiceScript->InitDiceRowCol(i + 1, j + 1);

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


#undef OBJECT