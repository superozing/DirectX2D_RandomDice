#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CDiceScript.h"



CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
{
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
	OBJECT->GetRenderComponent()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));
	OBJECT->Transform()->SetRelativePos(Vec3(0, -210, 500));
	OBJECT->Transform()->SetRelativeScale(Vec3(540, 540, 1));



	wstrPath = L"prefab\\AlphaBlendGameObject.pref";
	// MeshRender, Transform, alphablend shader, mtrl 달린 프리팹.
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

			// 주사위 종류과 정보 설정
			DiceScript->SetDiceWithInfo(DICE(i + 1));

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");

#pragma endregion

#pragma region _		Background Object




		}
	}

	////////////////////////////////
	// battlefield_normal_bottom_bg
	////////////////////////////////
	
	pObj = pObjPref->Instantiate();

	wstrPath = L"texture\\BattleField\\Deck\\battlefield_normal_bottom_bg.png";
	pObj->SetName(wstrPath);

	pObj->Transform()->SetRelativePos(Vec3(0.f, -425.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(510.f, 110.f, 1.f));

	pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");


	////////////////////////////////
	// battlefield_normal_bottom_bg
	////////////////////////////////

	//pObj = pObjPref->Instantiate();

	//wstrPath = L"texture\\BattleField\\Deck\\battlefield_normal_bottom_bg.png";
	//pObj->SetName(wstrPath);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 905.f, 0.f));
	//pObj->Transform()->SetRelativeScale(Vec3(510.f, 110.f, 1.f));

	//pObj->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	//CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObj, L"Background");

#pragma endregion




}

#undef OBJECT