#pragma once
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

class CameraUI :
    public ComponentUI
{
private:
    vector<string>      m_vecLayerName;

public:
    void CheckLayer(DWORD_PTR _ptr);

public:
    virtual void render_update() override;

public:
    CameraUI();
    ~CameraUI();
};

