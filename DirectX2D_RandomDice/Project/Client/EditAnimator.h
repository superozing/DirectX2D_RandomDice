#pragma once
#include "UI.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CAsset.h>
#include <Engine\CTexture.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnim.h>

#include <Windows.h>
#include <commdlg.h>
#include <DirectXMath.h>


class EditAnimator :
    public UI
{
private:
    CGameObject* m_TargetObject;
    vector<tAnimFrm>    m_vecAnimFrm;

    wstring         m_AnimName;
    Ptr<CTexture>   m_pAtlasTex;
    ImVec2          m_ImageSize;
    float           m_width;
    float           m_height;

    ComPtr<ID3D11ShaderResourceView>    m_textureID;
    ImTextureID                         m_imTexId;

    bool    m_IsPlaying;
    bool    m_Loop;
    bool    m_IsEdit;
    float   m_AccTime;
    int     m_CurFrame;

public:
    void SetTaretObject(CGameObject* _Object);
    void SetAnimName(string _strKey) { m_AnimName = ToWString(_strKey).c_str(); }

    CGameObject* GetTargetObject() { return m_TargetObject; }

public:
    void OpenFildDialog();
    void LoadSpriteFromFile(const wstring& filePath);
    wstring GetFileFromPath(const wstring& filePath);
    wstring GetFileNameFromPath(const wstring& filePath);

public:
    virtual void render_update() override;
    virtual void Deactivate() override;

    DirectX::SimpleMath::Vector2 ImVec2ToVector2(const ImVec2& imVec)
    {
        return { imVec.x, imVec.y };
    }


public:
    EditAnimator();
    ~EditAnimator();
};