#include "Framework.h"

Frame::Frame(wstring textureFile, Vector2 startUV, Vector2 endUV)
{
	material = new Material(textureFile);
	size = (endUV - startUV) * material->GetTexture()->GetSize();

	mesh = new Mesh();
	MakeMesh(startUV, endUV);
	mesh->CreateMesh();
}

Frame::Frame(wstring textureFile, float x, float y, float width, float height, Vector2 pos)
{
	material = new Material(textureFile);
	Vector2 textureSize = material->GetTexture()->GetSize();
	size = Vector2(width, height);

	Vector2 startUV = Vector2(x, y) / textureSize;
	Vector2 cutSize = size / textureSize;
	Vector2 endUV = startUV + cutSize;

	mesh = new Mesh();
	MakeMesh(startUV, endUV, pos);
	mesh->CreateMesh();
}

Frame::~Frame()
{
	delete material;
	delete mesh;
}

void Frame::Render()
{
	material->Set();
	mesh->Draw();
}

void Frame::MakeMesh(Vector2 startUV, Vector2 endUV, Vector2 pos)
{
	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	vector<Vertex>& vertices = mesh->GetVertices();

	vertices.emplace_back(left, top, startUV.x, startUV.y);
	vertices.emplace_back(right, top, endUV.x, startUV.y);
	vertices.emplace_back(left, bottom, startUV.x, endUV.y);
	vertices.emplace_back(right, bottom, endUV.x, endUV.y);

	vector<UINT>& indices = mesh->GetIndices();

	indices = { 0, 1, 2, 2, 1, 3 };
}
