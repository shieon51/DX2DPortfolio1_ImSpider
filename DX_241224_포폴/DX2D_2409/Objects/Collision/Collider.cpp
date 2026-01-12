#include "Framework.h"

Collider::Collider()
{
    material->SetShader(L"Shaders/Collider.hlsl");
    material->SetColor(0, 1, 0);
}

Collider::~Collider()
{
}

void Collider::Render()
{
    //GameObject::Render(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}

bool Collider::IsCollision(Collider* collider, Vector2* overlap)
{
    if (!IsActive()) return false;
    if (!collider->IsActive()) return false;

    switch (collider->type)
    {
    case Collider::Type::BOX:
        return IsBoxCollision((BoxCollider*)collider, overlap);        
    case Collider::Type::CIRCLE:
        return IsCircleCollision((CircleCollider*)collider, overlap);        
    case Collider::Type::LINE:
        return IsLineCollision((LineCollider*)collider, overlap);
    }

    return false;
}

bool Collider::IsBetweenPoint(const Vector2& start, const Vector2& end1, const Vector2& end2, const Vector2& point)
{
    Vector2 A = end1 - start;
    Vector2 B = end2 - start;
    Vector2 C = point - start;

    float crossA = Vector2::Cross(A, C);
    float crossB = Vector2::Cross(B, C);

    return crossA * crossB < 0;
}
