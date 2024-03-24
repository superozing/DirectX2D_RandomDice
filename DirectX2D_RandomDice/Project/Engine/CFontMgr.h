#pragma once
#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper"))
#endif


#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

enum class FONT_TYPE
{
    ARIAL,
    MAPLE,
    ALBA_SUPER,
    ALBA_MATTER,
    EXO2,
    END
};

struct FONTINFO
{
    wstring             WStr;       // 출력할 문자열
    float               fPosX;      // X Pos
    float               fPosY;      // Y Pos
    float               fFontSize;  // 폰트의 크기
    UINT                Color;      // 폰트의 색상. FONT_RGBA 매크로 함수를 사용하세요.
    FONT_TYPE           FontType;   // 폰트 타입. 어떤 폰트를 사용할 것인지
    FW1_TEXT_FLAG       TextFlag = FW1_RESTORESTATE;    // 텍스트 플래그. FW1_RESTORESTATE FW1_CENTER 자주 사용함
};

class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr);
private:
    IFW1Factory*        m_pFW1Factory;
    IFW1FontWrapper*    m_pFontWrapper[(UINT)FONT_TYPE::END];

    vector<FONTINFO>    m_VecRenderFont;

public:
    void init();

    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY
        , float _fFontSize, UINT _Color, FONT_TYPE _FontType, FW1_TEXT_FLAG _TextFlag = FW1_RESTORESTATE);

    void AddRenderFont(const FONTINFO& _FontInfo) { m_VecRenderFont.push_back(_FontInfo); }

    void render();

};

