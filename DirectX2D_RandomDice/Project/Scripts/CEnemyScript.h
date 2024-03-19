#pragma once
#include <Engine\CScript.h>
class CEnemyScript :
    public CScript
{
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CEnemyScript);
    CEnemyScript();
    CEnemyScript(const CEnemyScript& _Origin);
    ~CEnemyScript();
};

