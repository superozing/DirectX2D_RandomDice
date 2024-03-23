#pragma once
#include <Engine/CScript.h>
#include <Engine/global.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CUIScript :
    public CScript
{
private:

    bool            m_AllowSetRenderTexture;

    Ptr<CTexture>   m_NormalImg;
    Ptr<CTexture>   m_HoverImg;
    Ptr<CTexture>   m_CurImg;

    bool    m_bMouseOn;
    bool    m_bMouseOn_Prev;
    bool    m_bMouseLBtnDown;

    // 콜백 (전역함수 포인터) 
    BtnCallBack     m_CallBackFunc;
    // 델리게이트(맴버함수 포인터, 객체)
    CEntity* m_Inst;
    DelegateFunc    m_Delegate;
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetNormalImg(Ptr<CTexture> _NormalImg) { m_NormalImg = _NormalImg; }
    void SetHoverImg(Ptr<CTexture> _HoverImg) { m_HoverImg = _HoverImg; }

    void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

    // SetDeletage((CEntity*)m_pOwner->m_pGear, (DelegateFunc)&CGear_PlayLevel::PlayMusic);
    void SetDeletage(CEntity* _Inst, DelegateFunc _Func) { m_Inst = _Inst; m_Delegate = _Func; }
public:
    virtual void begin() override;
    virtual void tick() override;
    void render();

public:
    void OnHovered();
    void OnUnHovered();

    void LBtnDown();
    void LBtnUp();

    void LBtnReleased();
    void LBtnClicked();

    void AllowSetRenderTexture()    { m_AllowSetRenderTexture = true; }
    void DisallowSetRenderTexture() { m_AllowSetRenderTexture = false; }

public:
    CLONE(CUIScript);
    CUIScript();
    CUIScript(const CUIScript& _Other);
    ~CUIScript();
};