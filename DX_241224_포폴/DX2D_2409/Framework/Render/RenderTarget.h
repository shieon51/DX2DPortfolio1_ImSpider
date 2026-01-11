#pragma once

class RenderTarget
{
public:
    RenderTarget(UINT width = SCREEN_WIDTH, UINT height = SCREEN_HEIGHT);
    ~RenderTarget();

    void Set(Float4 clearColor = Float4(0, 0, 0, 0));

    ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
    void CreateRTVTexture();
    void CreateRTV();
    void CreateSRV();

private:
    UINT width, height;

    ID3D11Texture2D* rtvTexture;
    ID3D11RenderTargetView* rtv;
    ID3D11ShaderResourceView* srv;
};