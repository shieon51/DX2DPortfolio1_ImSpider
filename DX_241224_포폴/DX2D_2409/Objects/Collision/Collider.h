#pragma once

//class RectCollider;

class Collider : public GameObject
{
protected:
    enum class Type
    {
        BOX, CIRCLE, LINE
    };

public:
    Collider();
    ~Collider();    

    void Render();

    bool IsCollision(Collider* collider, Vector2* overlap = nullptr);

    virtual bool IsPointCollision(const Vector2& point) = 0;
    virtual bool IsBoxCollision(class BoxCollider* box, Vector2* overlap = nullptr) = 0;
    virtual bool IsCircleCollision(class CircleCollider* circle, Vector2* contactPoint = nullptr) = 0;
    virtual bool IsLineCollision(class LineCollider* line, Vector2* contactPoint = nullptr) = 0;

protected:
    bool IsBetweenPoint(const Vector2& start, const Vector2& end1,
        const Vector2& end2, const Vector2& point);

protected:
    Type type;
};
