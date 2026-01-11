#pragma once

class ImageObject : public BoxCollider
{
public:
    ImageObject(wstring textureFile, float maxFrameX = 1, float maxFrameY = 1);    
    ImageObject(wstring textureFile, Vector2 startUV, Vector2 endUV);
    ~ImageObject();

    void Update();
    void Render();

    void SetColor(float r, float g, float b, float a = 1.0f)
    {
        image->GetMaterial()->SetColor(r, g, b, a);
    }
    void SetColor(Float4 color)
    {
        image->GetMaterial()->SetColor(color);
    }

    Quad* GetImage() { return image; }

protected:
    Quad* image;
};
