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


void CFieldScript::begin()
{
	// 지금은 하드코딩으로 우겨넣지만
// 나중에는 프리팹을 가져와서 간단하게 넣어주기만 해도 되도록 만들어보자...

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Ptr<CPrefab> pDicePrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\DicePrefab.pref", L"prefab\\DicePrefab.pref");

			// 새로운 게임 오브젝트 할당
			CGameObject* pDice = m_DiceField[i][j] = pDicePrefab->Instantiate();

			pDice->MeshRender()->GetDynamicMaterial();

			// 주사위의 이름을 행과 열로 표시
			wstring DiceName = L"Dice_" + to_wstring(i) + to_wstring(j);
			pDice->SetName(DiceName);

			wstring CurDicePath = DicePathArr[(UINT)i + 1];

			Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(CurDicePath, CurDicePath);
			pDice->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

			// 게임 오브젝트에 주사위 스크립트 추가
			pDice->AddComponent(new CDiceScript);
			CDiceScript* DiceScript = pDice->GetScript<CDiceScript>();

			// 주사위 스크립트의 행렬 설정
			DiceScript->SetDiceRow(i);
			DiceScript->SetDiceCol(j);


			// 주사위 종류과 정보 설정
			DiceScript->SetDiceWithInfo(DICE(i + 1));

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");
		}
	}
}