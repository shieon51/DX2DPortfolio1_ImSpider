#include "Framework.h"

ImageObject::ImageObject(wstring textureFile, float maxFrameX, float maxFrameY)
{       
    image = new Quad(textureFile, Vector2(), Vector2(1/maxFrameX, 1/maxFrameY));
    image->SetParent(this);
    image->GetMaterial()->SetMaxFrame(maxFrameX, maxFrameY);
    
    UpdateSize(image->GetSize());
}

ImageObject::ImageObject(wstring textureFile, Vector2 startUV, Vector2 endUV)
{
    image = new Quad(textureFile, startUV, endUV);
    image->SetParent(this);

    UpdateSize(image->GetSize());
}

ImageObject::~ImageObject()
{
    delete image;
}

void ImageObject::Update()
{
    UpdateWorld();
    image->UpdateWorld();
}

void ImageObject::Render()
{
    image->Render();
    Collider::Render();
}
