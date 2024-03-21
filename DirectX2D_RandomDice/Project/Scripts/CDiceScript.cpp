#include "pch.h"
#include "CDiceScript.h"


CDiceScript::CDiceScript()
	:CScript(DICESCRIPT)
{

	// 주사위 종류
	AddScriptParam(SCRIPT_PARAM::INT, "Dice ", &Dice);

	// 행렬 상의 주사위 위치
	AddScriptParam(SCRIPT_PARAM::INT, "DiceRow :", &m_DiceRow);
	AddScriptParam(SCRIPT_PARAM::INT, "DiceCol :", &m_DiceCol);

	// 눈금 개수(등급)
	AddScriptParam(SCRIPT_PARAM::INT, "DiceScale :", &m_DiceScale);



}

CDiceScript::~CDiceScript()
{


}

#define OBJECT GetOwner()

void CDiceScript::SetDiceType(DICE _Dice)
{
}

void CDiceScript::SetDiceInfo(DICE _Dice)
{
}

void CDiceScript::SetDiceXY(UINT _DiceRow, UINT _DiceCol)
{
	m_DiceRow = _DiceRow;
	m_DiceCol = _DiceCol;

	Vec3 fieldPos = m_OwnerField->Transform()->GetRelativePos();


	OBJECT->SetName(L"Dice_" + to_wstring(m_DiceRow) + L"_" + to_wstring(m_DiceCol));

	Vec3 vvfieldPos = OBJECT->Transform()->GetRelativePos();

}

void CDiceScript::begin()
{
	if (OBJECT == nullptr)
		return;

	if (OBJECT->Transform() != nullptr)
	{
		OBJECT->Transform()->SetRelativeScale(Vec3(90.f, 90.f, 1.f));


	}

	if (OBJECT->MeshRender() != nullptr)
	{
		OBJECT->GetRenderComponent()->GetDynamicMaterial();
		wstring wstrPath = L"texture\\Dice\\01_fire.png";
		OBJECT->GetRenderComponent()->GetDynamicMaterial()
			->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(wstrPath, wstrPath));

	}
}

#undef OBJECT