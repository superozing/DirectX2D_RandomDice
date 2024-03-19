#pragma once
#include <Engine/CScript.h>

// 주사위 눈금 스크립트 - 눈금 투사체 관리
class CDiceScaleScript :
    public CScript
{
private:
    tParticleModule m_Module;



private:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    CLONE(CDiceScaleScript);
    CDiceScaleScript();
    CDiceScaleScript(const CDiceScaleScript& _Origin);
    ~CDiceScaleScript();
};

