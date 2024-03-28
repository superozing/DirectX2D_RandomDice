#pragma once
#include "ComponentUI.h"
class StateMachineUI :
    public ComponentUI
{
private:
    string          m_StateName;
    string          m_AddStateKey;
    wstring         m_CurStateKey;

public:
    void StateSelect(DWORD_PTR _ptr);

private:

public:
    virtual void render_update() override;
public:
    StateMachineUI();
    ~StateMachineUI();
};
