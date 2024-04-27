#pragma once
#include "CAsset.h"
#include "CState.h"
#include "CBlackboard.h"

class CStateMachine;

class CFSM :
    public CAsset
{
private:
    CFSM* m_Master;
    CStateMachine* m_StateMachine; // FSM 을 사용하는 StateMachine;

    map<wstring, CState*>   m_mapState;
    CBlackboard* m_Blackboard;

    CState* m_CurState;

public:
    const map<wstring, CState*>& GetStates() { return m_mapState; }
    CFSM* GetFSMIstance();
    CStateMachine* GetStateMachine() { return m_StateMachine; }
    void SetState(const wstring& _strState) { m_CurState = FindState(_strState); }
    void SetStateMachine(CStateMachine* _SM) { if (m_Master) { m_StateMachine = _SM; } }

public:
    CState* FindState(const wstring& _StateName);
    void AddState(const wstring& _StateName, CState* _State);
    void DeleteState(const wstring& _StateKey);
    void ChangeState(const wstring& _strStateName);

    virtual int Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

private:
    void ChangeState_proc(CState* _pNextState);

public:
    void finaltick();

public:
    CLONE_DISABLE(CFSM);

    CFSM(CFSM* _Origin, bool _bEngine = false);
    ~CFSM();


    friend class CTaskMgr;
    friend class CStateMachine;
};
