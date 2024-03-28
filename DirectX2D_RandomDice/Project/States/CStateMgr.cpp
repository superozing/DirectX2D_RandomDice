#include "pch.h"
#include "CStateMgr.h"

#include "CIdleState.h"
#include "CTestState.h"
#include "CTraceState.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CIdleState");
	_vec.push_back(L"CTestState");
	_vec.push_back(L"CTraceState");
}

CState* CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CIdleState" == _strStateName)
		return new CIdleState;
	if (L"CTestState" == _strStateName)
		return new CTestState;
	if (L"CTraceState" == _strStateName)
		return new CTraceState;
	return nullptr;
}

CState* CStateMgr::GetState(UINT _iStateType)
{
	switch (_iStateType)
	{
	case (UINT)STATE_TYPE::IDLESTATE:
		return new CIdleState;
		break;
	case (UINT)STATE_TYPE::TESTSTATE:
		return new CTestState;
		break;
	case (UINT)STATE_TYPE::TRACESTATE:
		return new CTraceState;
		break;
	}
	return nullptr;
}

const wchar_t * CStateMgr::GetStateName(CState * _pState)
{
	switch ((STATE_TYPE)_pState->GetStateType())
	{
	case STATE_TYPE::IDLESTATE:
		return L"CIdleState";
		break;

	case STATE_TYPE::TESTSTATE:
		return L"CTestState";
		break;

	case STATE_TYPE::TRACESTATE:
		return L"CTraceState";
		break;

	}
	return nullptr;
}