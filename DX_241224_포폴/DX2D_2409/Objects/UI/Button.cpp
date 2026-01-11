#include "Framework.h"

Button::Button(wstring textureFile)
    : ImageObject(textureFile)
{    
}

Button::~Button()
{
}

void Button::Update()
{
    if (!IsActive()) return;

    ClickEvent();
    ImageObject::Update();
}

void Button::Render()
{
    ImageObject::Render();

    if (text.size() == 0) return;

    FONT->SetColor(textColor);
    FONT->SetStyle(textStyle);
    FONT->RenderText(text, GetGlobalPos());
}

void Button::ClickEvent()
{
    if (IsPointCollision(mousePos))
    {
        if (KEY->Down(VK_LBUTTON))
            isDownCheck = true;

        if (KEY->Press(VK_LBUTTON))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && KEY->Up(VK_LBUTTON))
        {
            isDownCheck = false;

            if (event)
                event();

            if (intParamEvent)
                intParamEvent(intParam);

            if (objParamEvent)
                objParamEvent(objParam);
        }
    }
    else
    {
        state = NORMAL;

        if (KEY->Up(VK_LBUTTON))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NORMAL:
        SetColor(NORMAL_COLOR);
        break;
    case Button::OVER:
        SetColor(OVER_COLOR);
        break;
    case Button::DOWN:
        SetColor(DOWN_COLOR);
        break;    
    }
}
