#pragma once
#include "ComponentUI.h"
#include "ListUI.h"

#include <Engine/CAnim.h>


class Animator2DUI :
    public ComponentUI
{

    map<wstring, class CAnim*> m_MapAnims;
    vector<string> m_Keys;

    string m_selectedKey;

    vector<Vec2> m_offset;

    void SetMapAnims(map<wstring, class CAnim*> _map);

    Ptr<CTexture> SetImageForMakeAnim();

    Ptr<CTexture> m_curAtlasTex;

    float realWidth;
    float realHeight;

    int atlasDiv;

    int gridWidthCnt;
    int gridHeightCnt;

    bool s;
    bool bPushback;
    bool bPreviewBegin;

    bool setGrid;
    bool setClickedRect;
    bool bAtlasMakeStart;

    vector<ImVec2> addRectPos; // 0 : LeftTop, 1 : RightBottom 계속 반복

    vector<tAnimFrm> animFrms; // 생성되는 애님의 벡프레임

    bool curAnimRepeat;

    /*m_leftTop;
    m_Slice;
    m_Offset;
    m_BackGround;
    m_Duration;*/

    bool makeAnimStart;
    void MakeAnim();

    virtual void render_update() override;
public:
    Animator2DUI();
    ~Animator2DUI();
};

