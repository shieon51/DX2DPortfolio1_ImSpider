#pragma once

class BoxCollider : public Collider
{
public:
    struct ObbDesc
    {
        Vector2 pos;
        Vector2 axis[2];
        Vector2 halfSize;
    };

public:
    BoxCollider(Vector2 size = Vector2(50, 50));
    ~BoxCollider() = default;

    bool IsPointCollision(const Vector2& point) override;
    bool IsBoxCollision(BoxCollider* box, Vector2* overlap) override;
    bool IsCircleCollision(CircleCollider* circle, Vector2* contactPoint = nullptr) override;
    bool IsLineCollision(LineCollider* line, Vector2* contactPoint) override;

    Vector2 LeftTop() const;
    Vector2 LeftBottom() const;
    Vector2 RightTop() const;
    Vector2 RightBottom() const;

    float Left() const;
    float Right() const;
    float Top() const;
    float Bottom() const;

    void UpdateSize(Vector2 size);

    void GetObb(ObbDesc& box) const;

    Vector2 Size() const { return size * GetGlobalScale(); }
    Vector2 HalfSize() const { return Size() * 0.5f; }
    Vector2 OriginSize() const { return size; }

private:
    bool IsAABB(BoxCollider* box, Vector2* overlap);
    bool IsOBB(BoxCollider* box);    

    bool IsSeperate(const Vector2& seperateAxis, const ObbDesc& box1, const ObbDesc& box2);

private:
    Vector2 size;
};