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
	wstring wstrPath = L"prefab\\DefaultEnemy.pref";
	m_NormalEnemyPool.SetObject(CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath)->Instantiate());

	wstrPath = L"prefab\\BigEnemy.pref";
	m_BigEnemyPool.SetObject(CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath)->Instantiate());

	wstrPath = L"prefab\\SpeedEnemy.pref";
	m_SpeedEnemyPool.SetObject(CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath)->Instantiate());
}

CFieldScript::~CFieldScript()
{
}

#define OBJECT GetOwner()

void CFieldScript::begin()
{
	CGameObject* pObj = nullptr;
	wstring wstrPath;

	wstrPath = L"texture\\BattleField\\battlefield_normal_bg_top.png";
	OBJECT->GetRenderComponent()->GetDynamicMaterial();
	OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
	OBJECT->Transform()->SetRelativePos(Vec3(0, -210, 1000));
	OBJECT->Transform()->SetRelativeScale(Vec3(540, 540, 1));



	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	// MeshRender, Transform, alphablend shader, mtrl 달린 프리팹. 기능 없는 오브젝트에 사용 예정
	Ptr<CPrefab> pObjPref = CAssetMgr::GetInst()->Load<CPrefab>(wstrPath, wstrPath);

	
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

			// 스크립트 가져와서 저장
			CDiceScript* DiceScript = m_DiceField[i][j] = pDice->GetScript<CDiceScript>();

			DiceScript->begin();

			// 주사위 스크립트와 쌍방으로 연결
			DiceScript->SetField(this);

			// 주사위 스크립트의 행렬, Pos, Name 설정
			DiceScript->SetDiceXY(i + 1, j + 1);

			// 주사위 종류과 정보 설정 (z : 200)
			DiceScript->SetDiceType(DICE(i + 1));

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");

#pragma endregion

#pragma region _		Background Object

			//////////////////////////////////
			// battlefield_normal_dicebg_1 ~ 3
			//////////////////////////////////
	
			pObj = pObjPref->Instantiate();

			Vec3 dicePos = pDice->Transform()->GetRelativePos();
			
			pObj->Transform()->SetRelativePos(Vec3(dicePos.x, dicePos.y, dicePos.z + 100));
			pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

			wstrPath = L"texture\\BattleField\\Dice\\battlefield_normal_dicebg_" + to_wstring((i + j) % 3 + 1) + L".png";
			pObj->SetName(wstrPath);

			pObj->MeshRender()->GetDynamicMaterial();
			pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));


			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

		}
	}

	////////////////////////////////
	// battlefield_normal_bottom_bg
	////////////////////////////////
	
	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deck\\battlefield_normal_bottom_bg.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -425.f, 600.f));
	pObj->Transform()->SetRelativeScale(Vec3(510.f, 110.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");


	////////////////////////////////
	// battlefield_normal_vs_line_1
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-230.f, -160.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(33.f, 340.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////////////////
	// battlefield_normal_vs_line_2
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-10.f, 3.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(407.f, 19.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial();
	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////////////////
	// battlefield_normal_vs_line_3
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Line\\battlefield_normal_vs_line_3.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(216.f, -160.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(46.f, 343.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////////////////
	// battlefield_normal_deco_1
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_1.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(180.f, -280.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(75.f, 75.f, 1.f));
	pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, XM_PI / 3));

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////////////////
	// battlefield_normal_deco_2
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_2.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(189.f, -101.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 40.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////////////////
	// battlefield_normal_deco_4
	////////////////////////////////

	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deco\\battlefield_normal_deco_4.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(-190.f, -45.f, 700.f));
	pObj->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////
	// ingame_mob_gate 1
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(-235.f, -320.f, 600.f));

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_1");

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

	////////////////////
	// ingame_mob_gate 2
	////////////////////

	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->Transform()->SetRelativePos(Vec3(230.f, -320.f, 600.f));

	pObj->AddComponent(new CEnemyGateScript);
	pObj->SetName(L"ingame_mob_gate_2");

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

#pragma endregion




}

#undef OBJECT