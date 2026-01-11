#include "Framework.h"

unordered_map<wstring, Shader*> Shader::shaders;

Shader::Shader(wstring file)
{
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* blob;

    D3DCompileFromFile(file.c_str(), nullptr, nullptr,
        "PS", "ps_5_0", flags, 0, &blob, nullptr);

    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
        nullptr, &pixelShader);

    vertexShader = new VertexShader(file);
}

Shader::~Shader()
{
    pixelShader->Release();
    delete vertexShader;
}

Shader* Shader::Add(wstring file)
{
    if (shaders.count(file) > 0)
        return shaders[file];

    shaders[file] = new Shader(file);

    return shaders[file];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}

void Shader::Set()
{
    vertexShader->Set();
    deviceContext->PSSetShader(pixelShader, nullptr, 0);
}
