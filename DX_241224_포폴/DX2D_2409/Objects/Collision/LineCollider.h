#pragma once

class LineCollider : public Collider
{
public:
    LineCollider(Vector2 startPos, Vector2 endPos);

    bool IsPointCollision(const Vector2& point) override;
    bool IsBoxCollision(BoxCollider* box, Vector2* overlap) override;
    bool IsCircleCollision(CircleCollider* circle, Vector2* contactPoint = nullptr) override;
    bool IsLineCollision(LineCollider* line, Vector2* contactPoint) override;

    Vector2 Start() { return startPos * world; }
    Vector2 End() { return endPos * world; }

    Vector2 localStartPos() { return startPos; }
    Vector2 localEndPos() { return endPos; }
    //
    void UpdateStartAndEndPos(Vector2 startPos, Vector2 endPos);

private:
    Vector2 startPos, endPos;
    
};