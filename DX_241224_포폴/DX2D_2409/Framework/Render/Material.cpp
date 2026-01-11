#include "Framework.h"

Material::Material()
{
    Init();    
}

Material::Material(wstring textureFile)
{
    Init(); //처음 셰이더 지정
    texture = Texture::Add(textureFile);
}

Material::~Material()
{
    colorBuffer->Release();
}

void Material::Set()
{
    deviceContext->UpdateSubresource(colorBuffer, 0, nullptr, &color, 0, 0);
    deviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);

    deviceContext->UpdateSubresource(frameBuffer, 0, nullptr, &frame, 0, 0);
    deviceContext->PSSetConstantBuffers(1, 1, &frameBuffer);

    if(texture)
        texture->PSSet();

    shader->Set();
}

void Material::Edit()
{
    ImGui::ColorEdit4("Color", (float*)&color.tintColor);
}

void Material::SetShader(wstring shaderFile)
{
    shader = Shader::Add(shaderFile);
}

void Material::SetTexture(wstring file)
{
    texture = Texture::Add(file);
}

void Material::Init()
{
    shader = Shader::Add(L"Shaders/Texture.hlsl"); //처음 기본 셰이더

    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(ColorData);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        device->CreateBuffer(&bufferDesc, nullptr, &colorBuffer);
    }

    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(FrameData);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        device->CreateBuffer(&bufferDesc, nullptr, &frameBuffer);
    }
}
