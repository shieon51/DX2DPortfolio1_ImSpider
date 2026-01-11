#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void Render();
    void PostRender();

    void SetAlphaBlend();
    void SetAdditiveBlend();

    Camera* GetMainCamera() { return mainCamera; }
private:
    void CreateSamaplerState();
    void CreateBlendState();
    void CreateRasterizerState();
    void SetViewport();

private:
    ID3D11SamplerState* samplerState;
    ID3D11BlendState* alphaBlendState;
    ID3D11BlendState* additiveBlendState;
    ID3D11RasterizerState* rasterizerState;

    Camera* mainCamera;
};