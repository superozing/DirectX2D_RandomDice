#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdate.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    UINT                    m_MaxParticleCount;

    tParticleModule         m_Module;
    CStructuredBuffer*      m_ParticleModuleBuffer;

    CStructuredBuffer*      m_SpawnCountBuffer;

    Ptr<CParticleUpdate>    m_CSParticleUpdate;

    Ptr<CTexture>           m_ParticleTex;

    float                   m_Time;

    bool                    m_IsActivate;

public:

    void SetParticleModule(const tParticleModule& _Module) { m_Module = _Module; }
    void SetParticleTexture(Ptr<CTexture> _ParticleTex) { m_ParticleTex = _ParticleTex; }

    void SetActivate(bool _IsActivate) { m_IsActivate = _IsActivate; }
    bool GetActivate() const { return m_IsActivate; }

    virtual void UpdateData() override;
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _OriginParticle);
    ~CParticleSystem();
};

