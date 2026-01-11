#include "Framework.h"

GameObject::GameObject()
{    
    Init();    
}

GameObject::~GameObject()
{
    delete material;
    delete mesh;
    delete worldBuffer;
}

void GameObject::Render(D3D11_PRIMITIVE_TOPOLOGY type)
{
    if (!IsActive()) return;        

    SetWorld();
    material->Set();

    mesh->Draw(type);
}

void GameObject::SetWorld()
{
    worldBuffer->Set(world);
    worldBuffer->SetVS(1);
}

void GameObject::SetColor(float r, float g, float b, float a)
{
    material->SetColor(r, g, b, a);
}

void GameObject::Init()
{   
    mesh = new Mesh();
    material = new Material();
    worldBuffer = new MatrixBuffer();
}
