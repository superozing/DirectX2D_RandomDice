#pragma once
#include <Engine/CScript.h>

// �ֻ��� ���� ��ũ��Ʈ - ���� ����ü ����
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

