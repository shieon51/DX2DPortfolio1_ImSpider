#include "Framework.h"

Rope::Rope(Vector2 size) : size(size)
{
	MakeMesh(Vector2(), Vector2(1, 1));
	mesh->CreateMesh();
}

Rope::Rope(wstring textureFile, Vector2 startUV, Vector2 endUV)
{
	material->SetTexture(textureFile);
	size = material->GetTexture()->GetSize() * (endUV - startUV);
    originSize = size; //원래 이미지 사이즈

	MakeMesh(startUV, endUV);
	mesh->CreateMesh();
}

void Rope::UpdateSize(Vector2 size)
{
    this->size = size;

    vector<Vertex>& vertices = mesh->GetVertices();

    Vector2 uv = size / originSize; //현재 사이즈가 원래 이미지 사이즈에서 얼마나 변했나

    vertices.clear();

    //float halfX = size.x * 0.5f; //x 중심점은 맨 왼쪽으로 두기
    float halfY = size.y * 0.5f;
    vertices.emplace_back(0, -halfY, 0, uv.y);
    vertices.emplace_back(0, +halfY, 0, 0);
    vertices.emplace_back(-size.x, +halfY, uv.x, 0);
    vertices.emplace_back(-size.x, -halfY, uv.x, uv.y);

    mesh->UpdateVertices();
}

void Rope::Edit()
{
    Transform::Edit();
    if (ImGui::DragFloat2("Size", (float*)&size))
    {
        UpdateSize(size);
    }
}

void Rope::MakeMesh(Vector2 startUV, Vector2 endUV)
{
    vector<Vertex>& vertices = mesh->GetVertices();

    float halfX = size.x * 0.5f;
    float halfY = size.y * 0.5f;
    vertices.emplace_back(-halfX, -halfY, startUV.x, endUV.y);
    vertices.emplace_back(-halfX, +halfY, startUV.x, startUV.y);
    vertices.emplace_back(+halfX, +halfY, endUV.x, startUV.y);
    vertices.emplace_back(+halfX, -halfY, endUV.x, endUV.y);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1, 2, 0, 2, 3 };
}
