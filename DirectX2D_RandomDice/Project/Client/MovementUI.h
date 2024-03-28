#pragma once
#include "ComponentUI.h"
class MovementUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

    MovementUI();
    ~MovementUI();
};
