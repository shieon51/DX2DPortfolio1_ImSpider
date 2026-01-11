#pragma once

class Material
{
private:
    struct ColorData
    {
        Float4 tintColor = { 1, 1, 1, 1 };
        Float4 removeColor = { 1, 0, 1, 1 };
    };

    struct FrameData
    {
        Float2 curFrame = {};
        Float2 maxFrame = { 1, 1 };
    };

public:
    Material();
    Material(wstring textureFile);
    ~Material();

    void Set();   
    void Edit();

    void SetShader(wstring shaderFile);    

    void SetTexture(wstring file);
    void SetTexture(Texture* texture) { this->texture = texture; }
    void SetColor(float r, float g, float b, float a = 1.0f) { color.tintColor = { r, g, b, a }; }
    void SetColor(Float4 color) { this->color.tintColor = color; }
    void SetRemoveColor(float r, float g, float b, float a = 1.0f) { color.removeColor = { r, g, b, a }; }
    void SetRemoveColor(Float4 color) { this->color.removeColor = color; }
    void SetCurFrame(float x, float y) { frame.curFrame = { x, y }; }
    void SetMaxFrame(float x, float y) { frame.maxFrame = { x, y }; }
    void SetCurFrame(Vector2 curFrame) { frame.curFrame = curFrame; }
    void SetMaxFrame(Vector2 maxFrame) { frame.maxFrame = maxFrame; }

    FrameData GetFrame() { return frame; }

    Texture* GetTexture() { return texture; }    

private:
    void Init();

private:
    Shader* shader;
    Texture* texture = nullptr;

    ID3D11Buffer* colorBuffer;    
    ID3D11Buffer* frameBuffer;

    ColorData color;
    FrameData frame;
};