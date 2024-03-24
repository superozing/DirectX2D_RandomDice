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
    wstring             WStr;       // ����� ���ڿ�
    float               fPosX;      // X Pos
    float               fPosY;      // Y Pos
    float               fFontSize;  // ��Ʈ�� ũ��
    UINT                Color;      // ��Ʈ�� ����. FONT_RGBA ��ũ�� �Լ��� ����ϼ���.
    FONT_TYPE           FontType;   // ��Ʈ Ÿ��. � ��Ʈ�� ����� ������
    FW1_TEXT_FLAG       TextFlag = FW1_RESTORESTATE;    // �ؽ�Ʈ �÷���. FW1_RESTORESTATE FW1_CENTER ���� �����
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

