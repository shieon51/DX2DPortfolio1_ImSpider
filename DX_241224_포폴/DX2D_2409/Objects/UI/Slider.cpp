#include "Framework.h"

Slider::Slider(wstring frontImageFile, wstring backImageFile)
    : Quad(frontImageFile)
{
    material->SetShader(L"Shaders/Slider.hlsl");
    material->SetCurFrame(1, 0);

    backImage = Texture::Add(backImageFile);
}

Slider::~Slider()
{
}

void Slider::Render()
{
    backImage->PSSet(1);

    Quad::Render();
}

void Slider::SetAmount(float value)
{
    material->SetCurFrame(value, 0);
}
