#pragma once

class GameObject : public Transform
{
public:
    GameObject();
    ~GameObject();

    void Render(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Material* GetMaterial() { return material; }
    Mesh* GetMesh() { return mesh; }

    void SetWorld();
    void SetColor(float r, float g, float b, float a = 1.0f);

private:
    void Init();

protected:    
    Mesh* mesh;
    Material* material;
    MatrixBuffer* worldBuffer;
};