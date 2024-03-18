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
	// ������ �ϵ��ڵ����� ��ܳ�����
// ���߿��� �������� �����ͼ� �����ϰ� �־��ֱ⸸ �ص� �ǵ��� ������...

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			Ptr<CPrefab> pDicePrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\DicePrefab.pref", L"prefab\\DicePrefab.pref");

			// ���ο� ���� ������Ʈ �Ҵ�
			CGameObject* pDice = m_DiceField[i][j] = pDicePrefab->Instantiate();

			pDice->MeshRender()->GetDynamicMaterial();

			// �ֻ����� �̸��� ��� ���� ǥ��
			wstring DiceName = L"Dice_" + to_wstring(i) + to_wstring(j);
			pDice->SetName(DiceName);

			wstring CurDicePath = DicePathArr[(UINT)i + 1];

			Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(CurDicePath, CurDicePath);
			pDice->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

			// ���� ������Ʈ�� �ֻ��� ��ũ��Ʈ �߰�
			pDice->AddComponent(new CDiceScript);
			CDiceScript* DiceScript = pDice->GetScript<CDiceScript>();

			// �ֻ��� ��ũ��Ʈ�� ��� ����
			DiceScript->SetDiceRow(i);
			DiceScript->SetDiceCol(j);


			// �ֻ��� ������ ���� ����
			DiceScript->SetDiceWithInfo(DICE(i + 1));

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");
		}
	}
}