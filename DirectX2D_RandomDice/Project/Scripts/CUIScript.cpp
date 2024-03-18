#include "pch.h"
#include "CUIScript.h"
#include <Engine/CKeyMgr.h>

CUIScript::CUIScript()
	: CScript(UISCRIPT)
{
}

CUIScript::CUIScript(const CUIScript& _Other)
	: CScript(UISCRIPT)
{
}

CUIScript::~CUIScript()
{
}


void CUIScript::tick()
{
	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		
		Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
		Vec3 vWorldScale = GetOwner()->Transform()->GetWorldScale();

		Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, vWorldPos.y - vWorldScale.y / 2);
		Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, vWorldPos.y + vWorldScale.y / 2);
		
		if (vLT.x < vMousePos.x && vMousePos.x < vRB.x
			&& vLT.y < vMousePos.y && vMousePos.y < vRB.y)
		{
			LBtnClicked();
		}
	}

	if (m_Clicked && KEY_RELEASED(KEY::LBTN))
		LBtnReleased();

}

void CUIScript::LBtnReleased()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_NormalImg);
	m_Clicked = false;
}

void CUIScript::LBtnClicked()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_PressedImg);
	m_Clicked = true;

	// CallBack 존재 시 호출
	if (m_CallBackFunc) m_CallBackFunc();
		
	// Delegate 존재 시 호출
	if (m_Inst != nullptr && m_Delegate != nullptr)
		(m_Inst->*m_Delegate)();

}
