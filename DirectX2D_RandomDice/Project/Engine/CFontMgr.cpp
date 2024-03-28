#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

#include <vector>

CFontMgr::CFontMgr()
	: m_pFW1Factory(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_pFW1Factory)
		m_pFW1Factory->Release();

	// 모든 폰트 레퍼 해제
	for (auto &it : m_pFontWrapper)
		if (nullptr != it)
			it->Release();
}

void CFontMgr::init()
{
	wstring wstrPath = CPathMgr::GetContentPath();

	// 폰트 래퍼 팩토리 생성
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
		assert(NULL);

	// 폰트 래퍼 생성
	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper[(UINT)FONT_TYPE::ARIAL])))
		assert(NULL);

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Maplestory", &m_pFontWrapper[(UINT)FONT_TYPE::MAPLE])))
		assert(NULL);

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Alba Super", &m_pFontWrapper[(UINT)FONT_TYPE::ALBA_SUPER])))
		assert(NULL);

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Alba Matter", &m_pFontWrapper[(UINT)FONT_TYPE::ALBA_MATTER])))
		assert(NULL);

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Exo 2", &m_pFontWrapper[(UINT)FONT_TYPE::EXO2])))
		assert(NULL);

}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize
	, UINT _Color, FONT_TYPE _FontType, FW1_TEXT_FLAG _TextFlag)
{
	m_pFontWrapper[(UINT)_FontType]->DrawString(
		CONTEXT,
		_pStr,			// String
		_fFontSize,		// Font size
		_fPosX,			// X position
		_fPosY,			// Y position
		_Color,			// Text color, 0xAaBbGgRr
		_TextFlag       // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::render()
{
	// 윈도우 해상도 가져오기
	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	vResol.x /= 2;
	vResol.y /= 2;

	for (auto& it : m_VecRenderFont)
	{
		if (it.IsWorldPosRender == true)
		{
			// WorldPos -> 윈도우 좌표계로 변환 후 폰트 그리기
			DrawFont(it.WStr.c_str()
				, it.fPosX + vResol.x + it.WorldRenderOffset.x
				, -it.fPosY + vResol.y + it.WorldRenderOffset.y
				, it.fFontSize, it.Color, it.FontType, it.TextFlag);
		}
		else
		{
			DrawFont(it.WStr.c_str(), it.fPosX, it.fPosY, it.fFontSize, it.Color, it.FontType, it.TextFlag);
		}
	}

	m_VecRenderFont.clear();
}
