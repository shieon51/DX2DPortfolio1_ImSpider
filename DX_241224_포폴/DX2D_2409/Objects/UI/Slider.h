#pragma once

class Slider : public Quad
{
public:
    Slider(wstring frontImageFile, wstring backImageFile);
    ~Slider();

    void Render();

    void SetAmount(float value);

private:
    Texture* backImage;
};