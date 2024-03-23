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

struct FONTINFO
{
    wstring _pStr;
    float   _fPosX;
    float   _fPosY;
    float   _fFontSize;
    UINT    _Color; // FONT_RGBA 매크로 함수를 사용하세요.
};

class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr);
private:
    IFW1Factory*        m_pFW1Factory;
    IFW1FontWrapper*    m_pFontWrapper;

    vector<FONTINFO>    m_VecPrintFontBeforeUIRender;
    vector<FONTINFO>    m_VecPrintFontAfterUIRender;

public:
    void init();
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);

    void AddFontBeforeUIRender(const FONTINFO& _FontInfo) { m_VecPrintFontBeforeUIRender.push_back(_FontInfo); }
    void AddFontAfterUIRender(const FONTINFO& _FontInfo) { m_VecPrintFontAfterUIRender.push_back(_FontInfo); }

    void render_BeforeUI();
    void render_AfterUI();

};

