#pragma once
#include <Engine/CScript.h>

class CRotateScript :
	public CScript
{
private:

	Vec3 m_RotateValue;


public:
	virtual void SaveToFile(FILE* _File) {}
	virtual void LoadFromFile(FILE* _File) {}

	virtual void tick();

	void SetRotateValue(Vec3 _RotateValue) { m_RotateValue = _RotateValue; }
	Vec3 GetRotateValue(float _RotateValue) const { return m_RotateValue; }


public:
	CLONE(CRotateScript);
	CRotateScript();
	CRotateScript(const CRotateScript& _Origin);
	~CRotateScript();
};

