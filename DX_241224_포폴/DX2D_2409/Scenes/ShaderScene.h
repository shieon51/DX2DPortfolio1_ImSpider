#pragma once

class ShaderScene : public Scene
{
public:
    ShaderScene();
    ~ShaderScene();

    void Update() override;
    void Render() override;
    void PostRender() override;

private:
    Quad* quad;
    Texture* secondMap;

    IntValueBuffer* intValueBuffer;
    FloatValueBuffer* floatValueBuffer;
};