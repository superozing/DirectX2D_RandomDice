#pragma once
#include "ComponentUI.h"

#include <Engine\CGameObject.h>

class Animator2DUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

public:
    void OpenFildDialog();
    void LoadAnimationFromFile(const wstring& filePath);
    wstring GetFileFromPath(const wstring& filePath);

public:
    Animator2DUI();
    ~Animator2DUI();
};