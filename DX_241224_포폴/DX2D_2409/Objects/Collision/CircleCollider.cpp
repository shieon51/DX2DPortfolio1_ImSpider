#include "Framework.h"

CircleCollider::CircleCollider(float radius) : radius(radius)
{
    type = Type::CIRCLE;

    vector<Vertex>& vertices = mesh->GetVertices();
    
    float stepAngle = PI * 2.0f / VERTEX_SIZE;

    vertices.reserve(VERTEX_SIZE);    
    for (int i = 0; i < VERTEX_SIZE ; i++)
    {
        float x = cos(stepAngle * i) * radius;
        float y = sin(stepAngle * i) * radius;

        vertices.emplace_back(x, y, 1, 1);
    }

    vector<UINT>& indices = mesh->GetIndices();
    indices.reserve(VERTEX_SIZE + 1);
    for (int i = 0; i < VERTEX_SIZE; i++)
    {
        indices.push_back(i);
    }
    indices.push_back(0);

    mesh->CreateMesh();
}

bool CircleCollider::IsPointCollision(const Vector2& point)
{
    if (!IsActive()) return false;

    Vector2 dir = point - GetGlobalPos();

    return dir.Magnitude() <= Radius();
}

bool CircleCollider::IsBoxCollision(BoxCollider* box, Vector2* overlap)
{
    return box->IsCircleCollision(this);
}

bool CircleCollider::IsCircleCollision(CircleCollider* circle, Vector2* contactPoint)
{
    float distance = Vector2::Distance(GetGlobalPos(), circle->GetGlobalPos());

    return distance <= Radius() + circle->Radius();
}

bool CircleCollider::IsLineCollision(LineCollider* line, Vector2* contactPoint)
{
    return false;
}
