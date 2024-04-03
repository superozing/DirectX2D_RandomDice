#include "pch.h"
#include "CTraceState.h"


#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>

#include <Engine/CTimeMgr.h>

CTraceState::CTraceState()
	: CState(STATE_TYPE::TRACESTATE)
{
}

CTraceState::~CTraceState()
{
}


void CTraceState::finaltick()
{
	// �÷��̾ ��ó�� ������, Trace ���·� �����Ѵ�.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	CGameObject* pTarget = ((CGameObject*)GetBlackboardData(L"TargetObject"));
	CGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	pSelf->Transform()->SetRelativePos(vPos);
}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}