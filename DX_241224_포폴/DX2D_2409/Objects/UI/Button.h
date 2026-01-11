#pragma once

class Button : public ImageObject
{
protected:
    enum State
    {
        NORMAL, OVER, DOWN
    };

    const Float4 NORMAL_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    const Float4 OVER_COLOR = { 0.9f, 0.9f, 0.9f, 1.0f };
    const Float4 DOWN_COLOR = { 0.5f, 0.5f, 0.5f, 1.0f };

public:
    Button(wstring textureFile = L"Textures/UI/Pack/button_square.png");
    ~Button();

    void Update();
    void Render();    

    void SetEvent(function<void()> event) { this->event = event; }
    void SetIntParamEvent(function<void(int)> event) { intParamEvent = event; }
    void SetObjParamEvent(function<void(void*)> event) { objParamEvent = event; }

    void SetIntParam(int value) { intParam = value; }
    void SetObjParam(void* value) { objParam = value; }

    void SetText(string text) { this->text = text; }
    void SetTextColor(string colorKey) { textColor = colorKey; }
    void SetTextStyle(string styleKey) { textStyle = styleKey; }
private:
    void ClickEvent();

protected:
    State state = NORMAL;
    bool isDownCheck = false;

    string text;
    string textColor = "Black";
    string textStyle = "Button";

    function<void()> event = nullptr;
    function<void(int)> intParamEvent = nullptr;
    function<void(void*)> objParamEvent = nullptr;

    int intParam = 0;
    void* objParam = nullptr;
};