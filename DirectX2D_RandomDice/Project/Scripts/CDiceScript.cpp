#include "pch.h"
#include "CDiceScript.h"


CDiceScript::CDiceScript()
	:CScript(DICESCRIPT)
{

	// �ֻ��� ����
	AddScriptParam(SCRIPT_PARAM::INT, "Dice ", &Dice);

	// ��� ���� �ֻ��� ��ġ
	AddScriptParam(SCRIPT_PARAM::INT, "DiceRow :", &m_DiceRow);
	AddScriptParam(SCRIPT_PARAM::INT, "DiceCol :", &m_DiceCol);

	// ���� ����(���)
	AddScriptParam(SCRIPT_PARAM::INT, "Grade :", &m_DiceGrade);



}

CDiceScript::~CDiceScript()
{


}

#define OBJECT GetOwner()

void CDiceScript::SetDiceWithInfo(DICE _Dice)
{
	Dice = _Dice;

	OBJECT->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0
		, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Dice\\01_fire.png", L"texture\\Dice\\01_fire.png"));
}

void CDiceScript::SetDiceXY(UINT _DiceRow, UINT _DiceCol)
{
	m_DiceRow = _DiceRow;
	m_DiceCol = _DiceCol;

	Vec3 fieldPos = m_OwnerField->Transform()->GetRelativePos();

	OBJECT->Transform()->SetRelativePos(Vec3(((int)m_DiceRow - 3) * 62, ((int)m_DiceCol - 3) * 62 - 90, 3));

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

	}
}

#undef OBJECT