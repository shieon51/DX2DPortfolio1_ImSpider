#pragma once

class Shader
{
private:
    Shader(wstring file);
    ~Shader();

public:
    static Shader* Add(wstring file);
    static void Delete();

    void Set();

private:
    VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    

    static unordered_map<wstring, Shader*> shaders;
};