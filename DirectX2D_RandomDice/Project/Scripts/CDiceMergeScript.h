#pragma once
#include <Engine/CScript.h>
class CDiceMergeScript :
    public CScript
{
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceMergeScript);
    CDiceMergeScript();
    CDiceMergeScript(const CDiceMergeScript& _Origin);
    ~CDiceMergeScript();
};

