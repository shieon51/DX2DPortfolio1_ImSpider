#pragma once

class CircleCollider : public Collider
{
private:
    const UINT VERTEX_SIZE = 64;

public:
    CircleCollider(float radius = 50.0f);

    bool IsPointCollision(const Vector2& point) override;
    bool IsBoxCollision(BoxCollider* box, Vector2* overlap) override;
    bool IsCircleCollision(CircleCollider* circle, Vector2* contactPoint = nullptr) override;
    bool IsLineCollision(LineCollider* line, Vector2* contactPoint) override;

    float Radius() { return radius * max(GetGlobalScale().x, GetGlobalScale().y); }

private:
    float radius;

    // Collider을(를) 통해 상속됨
    
};