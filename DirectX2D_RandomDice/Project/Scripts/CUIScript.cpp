#include "pch.h"
#include "CUIScript.h"
#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CAssetMgr.h>
#include <Engine\CDevice.h>
CUIScript::CUIScript()
	: CScript(UISCRIPT)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CUIScript::CUIScript(const CUIScript& _Other)
	: CScript(UISCRIPT)
	, m_NormalImg(nullptr)
	, m_HoverImg(nullptr)
	, m_bMouseOn(false)
	, m_bMouseOn_Prev(false)
	, m_bMouseLBtnDown(false)
{
}

CUIScript::~CUIScript()
{
}
void CUIScript::begin()
{
	m_CurImg = m_NormalImg;
}

void CUIScript::tick()
{
	m_bMouseOn_Prev = m_bMouseOn;

	Vec2 vWMousePos = CKeyMgr::GetInst()->GetMouseWorldPos();

	Vec3 vWorldPos = GetOwner()->Transform()->GetWorldPos();
	Vec3 vWorldScale = GetOwner()->Transform()->GetWorldScale();

	Vec2 vLT = Vec2(vWorldPos.x - vWorldScale.x / 2, vWorldPos.y - vWorldScale.y / 2);
	Vec2 vRB = Vec2(vWorldPos.x + vWorldScale.x / 2, vWorldPos.y + vWorldScale.y / 2);

	if (vLT.x < vWMousePos.x && vWMousePos.x < vRB.x
		&& vLT.y < vWMousePos.y && vWMousePos.y < vRB.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	bool bLBtnTap = KEY_TAP(LBTN);
	bool bLbtnReleased = KEY_RELEASED(LBTN);

	if (m_bMouseOn)
	{
		if (m_bMouseOn_Prev != m_bMouseOn)
			OnHovered();

		if (bLbtnReleased)
		{
			LBtnUp();

			if (m_bMouseLBtnDown)
			{
				LBtnClicked();
			}
		}

		if (bLbtnReleased)
			m_bMouseLBtnDown = false;
	}
	else
	{
		if (m_bMouseOn_Prev != m_bMouseOn)
			OnUnHovered();
	}

	render();
}

void CUIScript::render()
{
	GetOwner()->MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_CurImg);
}

void CUIScript::OnHovered()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_HoverImg;
}

void CUIScript::OnUnHovered()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_NormalImg;
}

void CUIScript::LBtnDown()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_NormalImg;
}

void CUIScript::LBtnUp()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_NormalImg;
}

void CUIScript::LBtnReleased()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_NormalImg;

	m_bMouseLBtnDown = false;
}

void CUIScript::LBtnClicked()
{
	if (m_AllowSetRenderTexture)
		m_CurImg = m_NormalImg;

	// CallBack
	if (m_CallBackFunc != nullptr) 
		m_CallBackFunc();
	
	// Delegate
	if (m_Inst != nullptr && m_Delegate != nullptr)
		(m_Inst->*m_Delegate)();
}