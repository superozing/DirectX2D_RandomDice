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

	// ����� 1 �ö� ������ ������ �ֻ����� ����
	pDice->SetDiceType(pField->GetRandomDeckDiceType(), pDice->GetDiceScale() + 1);

	// ���� �ڸ��� �ֻ��� ����
	_FocusDice->SetDiceType(DICE::NONE);
}

bool CDefaultMerge::DefaultIsMergeAbleDelegateFunc(CDiceScript* _FocusDice)
{
	// ���� �ֻ��� �����̸鼭 ���� ������ ��� true
	return _FocusDice->GetDice() == GetOwnerDice()->GetDice() && _FocusDice->GetDiceScale() == GetOwnerDice()->GetDiceScale();
}
