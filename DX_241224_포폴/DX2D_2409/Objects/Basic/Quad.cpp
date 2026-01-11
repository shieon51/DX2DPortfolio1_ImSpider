#include "Framework.h"

Quad::Quad(Vector2 size) : size(size)
{
    MakeMesh(Vector2(), Vector2(1, 1));
    mesh->CreateMesh();
}

Quad::Quad(wstring textureFile, Vector2 startUV, Vector2 endUV)
{
    material->SetTexture(textureFile);
    size = material->GetTexture()->GetSize() * (endUV - startUV);

    MakeMesh(startUV, endUV);
    mesh->CreateMesh();
}

void Quad::UpdateSize(Vector2 size)
{
    this->size = size;

    vector<Vertex>& vertices = mesh->GetVertices();

    vertices.clear();

    float halfX = size.x * 0.5f;
    float halfY = size.y * 0.5f;
    vertices.emplace_back(-halfX, -halfY, 0, 1);
    vertices.emplace_back(-halfX, +halfY, 0, 0);
    vertices.emplace_back(+halfX, +halfY, 1, 0);
    vertices.emplace_back(+halfX, -halfY, 1, 1);

    mesh->UpdateVertices();
}

void Quad::Edit()
{
    Transform::Edit();
}

void Quad::MakeMesh(Vector2 startUV, Vector2 endUV)
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
