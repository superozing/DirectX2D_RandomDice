#include "pch.h"
#include "CRotateScript.h"

CRotateScript::CRotateScript()
	:CScript(SCRIPT_TYPE::ROTATESCRIPT)
{
}

CRotateScript::CRotateScript(const CRotateScript& _Origin)
	:CScript(_Origin.GetScriptType())
{
}

CRotateScript::~CRotateScript()
{
}

#define OBJ GetOwner()

void CRotateScript::tick()
{
	if (OBJ != nullptr)
	{
		Vec3 Rot = OBJ->Transform()->GetRelativeRotation();

		Rot.x += m_RotateValue.x * DT;
		Rot.y += m_RotateValue.y * DT;
		Rot.z += m_RotateValue.z * DT;

		OBJ->Transform()->SetRelativeRotation(Rot);
	}
}

#undef OBJ
