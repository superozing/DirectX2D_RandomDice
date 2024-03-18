#pragma once
#include <Engine/CScript.h>
#include <Engine/global.h>

typedef  void(*BtnCallBack)(void);
typedef  void(CEntity::* DelegateFunc)(void);

class CUIScript :
    public CScript
{
private:
    Ptr<CTexture>   m_NormalImg;
    Ptr<CTexture>   m_PressedImg;

    bool            m_Clicked;

    // �ݹ� (�����Լ� ������) 
    BtnCallBack     m_CallBackFunc;

    // ��������Ʈ(�ɹ��Լ� ������, ��ü)
    CEntity*        m_Inst;
    DelegateFunc    m_Delegate;

    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File) override {}

public:
    void SetNormalImg(Ptr<CTexture> _NormalImg) { m_NormalImg = _NormalImg; }
    void SetPressedImg(Ptr<CTexture> _PressedImg) { m_PressedImg = _PressedImg; }

    void SetCallBack(BtnCallBack _CallBack) { m_CallBackFunc = _CallBack; }

    // SetDeletage((CEntity*)m_pOwner->m_pGear, (DelegateFunc)&CGear_PlayLevel::PlayMusic);
    void SetDeletage(CEntity* _Inst, DelegateFunc _Func) { m_Inst = _Inst; m_Delegate = _Func; }

public:
    virtual void tick() override;

public:
    void LBtnReleased();
    void LBtnClicked();


public:
    CLONE(CUIScript);
    CUIScript();
    CUIScript(const CUIScript& _Other);
    ~CUIScript();
};

