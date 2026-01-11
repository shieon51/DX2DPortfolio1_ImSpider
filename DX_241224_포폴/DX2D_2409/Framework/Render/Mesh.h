#pragma once

struct Vertex
{
    float x = 0, y = 0, z = 0;
    float u = 0, v = 0;

    Vertex() = default;
    Vertex(float x, float y) : x(x), y(y)
    {}
    Vertex(float x, float y, float u, float v)
        : x(x), y(y), u(u), v(v)
    {}
};

struct InstanceData
{
    Matrix transform;
    Float2 curFrame;
    Float2 maxFrame;
};

class Mesh
{
public:
    Mesh() = default;
    ~Mesh();

    void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    void SetMesh(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    void CreateMesh();

    void UpdateVertices();    

    vector<Vertex>& GetVertices() { return vertices; }
    vector<UINT>& GetIndices() { return indices; }

private:
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;

    vector<Vertex> vertices;
    vector<UINT> indices;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
};