#pragma once
#include "ComponentUI.h"

void TextBox(const char* text, float width = 150);


class MovementUI :
    public ComponentUI
{
public:
    virtual void render_update() override;

    MovementUI();
    ~MovementUI();
};
