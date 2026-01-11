#pragma once

class RenderTargetScene : public Scene
{
public:
    RenderTargetScene();
    ~RenderTargetScene();

    void Update() override;

    void PreRender() override;
    void Render() override;
    void PostRender() override;

private:
    Quad* bg;
    Robot* robot;

    RenderTarget* renderTarget;
    Quad* renderTextures[4];

    IntValueBuffer* valueBuffer;
    FloatValueBuffer* floatValueBuffer;
};