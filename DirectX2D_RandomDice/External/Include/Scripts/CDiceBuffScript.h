#pragma once
#include <Engine/CScript.h>
class CDiceBuffScript :
    public CScript
{
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceBuffScript);
    CDiceBuffScript();
    CDiceBuffScript(const CDiceBuffScript& _Origin);
    ~CDiceBuffScript();
};

