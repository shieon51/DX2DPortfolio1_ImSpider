#pragma once

class VertexShader
{
private:
	friend class Shader;

	VertexShader(wstring file);
	~VertexShader();

public:
	void Set();

private:
	void CreateInputLayout();

private:
	ID3DBlob* blob;
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;

	ID3D11ShaderReflection* reflection;
};