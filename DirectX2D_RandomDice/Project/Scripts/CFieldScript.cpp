#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CDiceScript.h"

CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
{
	// 지금은 하드코딩으로 우겨넣지만
	// 나중에는 프리팹을 가져와서 간단하게 넣어주기만 해도 되도록 만들어보자...
	
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			// 새로운 게임 오브젝트 할당
			CGameObject* pDice = m_DiceField[i][j] = new CGameObject;

			// 주사위의 이름을 행과 열로 표시
			wstring DiceName = L"Dice_" + to_wstring(i) + to_wstring(j);
			pDice->SetName(DiceName);

			pDice->AddComponent(new CTransform);
			pDice->Transform()->SetRelativePos(Vec3(-590.f + i * 236.f, -354.f + j * 236.f, 200.f));
			pDice->Transform()->SetRelativeScale(Vec3(236.f, 236.f, 1.f));

			pDice->AddComponent(new CMeshRender);
			pDice->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pDice->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

			Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\01_fire.png", L"texture\\Dice\\01_fire.png");
			pDice->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

			// 게임 오브젝트에 주사위 스크립트 추가
			pDice->AddComponent(new CDiceScript);
			CDiceScript* DiceScript = pDice->GetScript<CDiceScript>();

			// 주사위 스크립트의 행렬 설정
			DiceScript->SetDiceRow(i);
			DiceScript->SetDiceCol(j);

			
			// 주사위 종류과 정보 설정
			DiceScript->SetDiceWithInfo(DICE::NONE);

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");
		}
	}

}

CFieldScript::~CFieldScript()
{
}
