#include "Framework.h"

Poly::Poly(int edge, float size)
    : edge(edge), size(size)
{
    MakeMesh();
    mesh->CreateMesh();
}

Poly::Poly(int edge, wstring textureFile)
{
    material->SetTexture(textureFile);
    Vector2 textureSize = material->GetTexture()->GetSize();

    size = min(textureSize.x, textureSize.y);

    MakeMesh();
    mesh->CreateMesh();
}

void Poly::MakeMesh()
{
    vector<Vertex>& vertices = mesh->GetVertices();

    float angle = 0.0f;
    float stepAngle = PI * 2.0f / edge;

    vertices.reserve(edge + 2);
    vertices.emplace_back(0, 0, 1, 1);

    Vector2 start(0.5f, 0.5f);
    for (int i = 0; i <= edge; i++)
    {
        Vector2 uv(cos(angle), sin(angle));

        float x = uv.x * size;
        float y = uv.y * size;
        
        uv = (uv * 0.5f) + Vector2(0.5f, 0.5f);

        vertices.emplace_back(x, y, uv.x, uv.y);       

        angle -= stepAngle;
    }

    vector<UINT>& indices = mesh->GetIndices();
    indices.reserve(edge * 3);
    for (int i = 0; i < edge; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}
