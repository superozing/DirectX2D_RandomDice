#include "pch.h"
#include "CFieldScript.h"

#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CDiceScript.h"

CFieldScript::CFieldScript()
	:CScript(FIELDSCRIPT)
{
	// ������ �ϵ��ڵ����� ��ܳ�����
	// ���߿��� �������� �����ͼ� �����ϰ� �־��ֱ⸸ �ص� �ǵ��� ������...
	
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			// ���ο� ���� ������Ʈ �Ҵ�
			CGameObject* pDice = m_DiceField[i][j] = new CGameObject;

			// �ֻ����� �̸��� ��� ���� ǥ��
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

			// ���� ������Ʈ�� �ֻ��� ��ũ��Ʈ �߰�
			pDice->AddComponent(new CDiceScript);
			CDiceScript* DiceScript = pDice->GetScript<CDiceScript>();

			// �ֻ��� ��ũ��Ʈ�� ��� ����
			DiceScript->SetDiceRow(i);
			DiceScript->SetDiceCol(j);

			
			// �ֻ��� ������ ���� ����
			DiceScript->SetDiceWithInfo(DICE::NONE);

			// AddObject
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pDice, L"Dice");
		}
	}

}

CFieldScript::~CFieldScript()
{
}
