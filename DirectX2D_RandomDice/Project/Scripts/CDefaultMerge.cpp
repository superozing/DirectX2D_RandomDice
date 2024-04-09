#include "pch.h"
#include "CDefaultMerge.h"
#include "CDiceScript.h"
#include "CFieldScript.h"

CDefaultMerge::CDefaultMerge()
	: CDiceMergeScript(DEFAULTMERGE)
{
	CDiceMergeScript::SetMergeDelegate((DiceMergeDelegate)&CDefaultMerge::DefaultMergeDiceDelegateFunc);
	CDiceMergeScript::SetIsMergeAbleDelegate((IsMergeAbleDelegate)&CDefaultMerge::DefaultIsMergeAbleDelegateFunc);
}

CDefaultMerge::CDefaultMerge(const CDefaultMerge& _Origin)
	: CDiceMergeScript(_Origin.GetScriptType())
{
	CDiceMergeScript::SetMergeDelegate((DiceMergeDelegate)&CDefaultMerge::DefaultMergeDiceDelegateFunc);
	CDiceMergeScript::SetIsMergeAbleDelegate((IsMergeAbleDelegate)&CDefaultMerge::DefaultIsMergeAbleDelegateFunc);
}

CDefaultMerge::~CDefaultMerge()
{
}

void CDefaultMerge::DefaultMergeDiceDelegateFunc(CDiceScript* _FocusDice)
{
	CDiceScript* pDice = GetOwnerDice();
	CFieldScript* pField = pDice->GetField();

	// 등급이 1 올라간 랜덤한 종류의 주사위로 변경
	pDice->SetDiceType(pField->GetRandomDeckDiceType(), pDice->GetDiceScale() + 1);

	// 기존 자리의 주사위 해제
	_FocusDice->SetDiceType(DICE::NONE);
}

bool CDefaultMerge::DefaultIsMergeAbleDelegateFunc(CDiceScript* _FocusDice)
{
	// 같은 주사위 종류이면서 같은 눈금일 경우 true
	return _FocusDice->GetDice() == GetOwnerDice()->GetDice() && _FocusDice->GetDiceScale() == GetOwnerDice()->GetDiceScale();
}
