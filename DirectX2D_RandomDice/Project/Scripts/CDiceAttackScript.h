#pragma once
#include <Engine/CScript.h>
class CDiceAttackScript :
    public CScript
{
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceAttackScript);
    CDiceAttackScript();
    CDiceAttackScript(const CDiceAttackScript& _Origin);
    ~CDiceAttackScript();
};

