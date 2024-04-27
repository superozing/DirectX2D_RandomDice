#pragma once
#include <Engine/CScript.h>

class CEnemyGateScript :
    public CScript
{
private:
	CGameObject* m_Background;
	CGameObject* m_pParticleObject;
	CParticleSystem* m_ParticleSystem;


public:
	virtual void SaveToFile(FILE* _File) {}
	virtual void LoadFromFile(FILE* _File) {}

	virtual void begin();

public:
	CLONE(CEnemyGateScript);
	CEnemyGateScript();
	CEnemyGateScript(const CEnemyGateScript& _Origin);
	~CEnemyGateScript();
};

