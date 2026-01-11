#include "Framework.h"

BoxCollider::BoxCollider(Vector2 size) : size(size)
{
    tag = "Box";
    type = Type::BOX;

    vector<Vertex>& vertices = mesh->GetVertices();

    float halfX = size.x * 0.5f;
    float halfY = size.y * 0.5f;
    vertices.emplace_back(-halfX, -halfY, 0, 1);
    vertices.emplace_back(-halfX, +halfY, 0, 0);
    vertices.emplace_back(+halfX, +halfY, 1, 0);
    vertices.emplace_back(+halfX, -halfY, 1, 1);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1, 2, 3, 0};

    mesh->CreateMesh();
}

//bool BoxCollider::IsPointCollision(const Vector2& point)
//{
//    Vector2 coord = point * XMMatrixInverse(nullptr, world);
//
//    if (abs(coord.x) < size.x * 0.5f && abs(coord.y) < size.y * 0.5f)
//        return true;
//
//    return false;
//}

bool BoxCollider::IsPointCollision(const Vector2& point)
{
    ObbDesc box;
    GetObb(box);

    Vector2 d = box.pos - point;

    float a = abs(Vector2::Dot(d, box.axis[0]));
    float b = abs(Vector2::Dot(d, box.axis[1]));

    return a < box.halfSize.x && b < box.halfSize.y;
}

//bool BoxCollider::IsPointCollision(const Vector2& point)
//{    
//    bool isLeftTop = IsBetweenPoint(LeftTop(), RightTop(), LeftBottom(), point);
//    bool isRightTop = IsBetweenPoint(RightTop(), LeftTop(), RightBottom(), point);
//    bool isRightBottom = IsBetweenPoint(RightBottom(), LeftBottom(), RightTop(), point);
//
//    return isLeftTop && isRightTop && isRightBottom;
//}

bool BoxCollider::IsBoxCollision(BoxCollider* box, Vector2* overlap)
{
    if (overlap)
        return IsAABB(box, overlap);

    return IsOBB(box);
}

bool BoxCollider::IsCircleCollision(CircleCollider* circle, Vector2* contactPoint)
{
    ObbDesc obb;
    GetObb(obb);

    Vector2 direction = obb.pos - circle->GetGlobalPos();

    float x = abs(Vector2::Dot(direction, obb.axis[0]));
    float y = abs(Vector2::Dot(direction, obb.axis[1]));

    if (x > obb.halfSize.x + circle->Radius()) return false;
    if (y > obb.halfSize.y + circle->Radius()) return false;

    if (x < obb.halfSize.x) return true;
    if (y < obb.halfSize.y) return true;

    Vector2 closestToCircleDir = Vector2(x, y) - obb.halfSize;

    return circle->Radius() > closestToCircleDir.Magnitude();
}

bool BoxCollider::IsLineCollision(LineCollider* line, Vector2* contactPoint)
{
    return line->IsBoxCollision(this, contactPoint);
}

Vector2 BoxCollider::LeftTop() const
{
    Vector2 edge = Vector2(-size.x, +size.y) * 0.5f;

    return edge * world;
}

Vector2 BoxCollider::LeftBottom() const
{
    Vector2 edge = Vector2(-size.x, -size.y) * 0.5f;

    return edge * world;
}

Vector2 BoxCollider::RightTop() const
{
    Vector2 edge = Vector2(+size.x, +size.y) * 0.5f;

    return edge * world;
}

Vector2 BoxCollider::RightBottom() const
{
    Vector2 edge = Vector2(+size.x, -size.y) * 0.5f;

    return edge * world;
}

float BoxCollider::Left() const
{
    float minLeft = min(LeftTop().x, LeftBottom().x);
    float minRight = min(RightTop().x, RightBottom().x);

    return min(minLeft, minRight);
}

float BoxCollider::Right() const
{
    float minLeft = max(LeftTop().x, LeftBottom().x);
    float minRight = max(RightTop().x, RightBottom().x);

    return max(minLeft, minRight);
}

float BoxCollider::Top() const
{
    float minTop = max(LeftTop().y, LeftBottom().y);
    float minBottom = max(RightTop().y, RightBottom().y);

    return max(minTop, minBottom);
}

float BoxCollider::Bottom() const
{
    float minTop = min(LeftTop().y, LeftBottom().y);
    float minBottom = min(RightTop().y, RightBottom().y);

    return min(minTop, minBottom);
}

void BoxCollider::UpdateSize(Vector2 size)
{
    this->size = size;

    vector<Vertex>& vertices = mesh->GetVertices();

    vertices.clear();

    float halfX = size.x * 0.5f;
    float halfY = size.y * 0.5f;
    vertices.emplace_back(-halfX, -halfY, 0, 1);
    vertices.emplace_back(-halfX, +halfY, 0, 0);
    vertices.emplace_back(+halfX, +halfY, 1, 0);
    vertices.emplace_back(+halfX, -halfY, 1, 1);

    mesh->UpdateVertices();
}

void BoxCollider::GetObb(ObbDesc& box) const
{
    box.pos = GetGlobalPos();
    box.halfSize = HalfSize();

    box.axis[0] = GetRight();
    box.axis[1] = GetUp();
}

bool BoxCollider::IsAABB(BoxCollider* box, Vector2* overlap)
{
    float left = max(Left(), box->Left());
    float right = min(Right(), box->Right());
    float bottom = max(Bottom(), box->Bottom());
    float top = min(Top(), box->Top());

    overlap->x = right - left;
    overlap->y = top - bottom;

    return overlap->x > 0 && overlap->y > 0;
}

bool BoxCollider::IsOBB(BoxCollider* box)
{
    ObbDesc obbA, obbB;
    GetObb(obbA);
    box->GetObb(obbB);

    if (IsSeperate(obbA.axis[0], obbA, obbB)) return false;
    if (IsSeperate(obbA.axis[1], obbA, obbB)) return false;
    if (IsSeperate(obbB.axis[0], obbA, obbB)) return false;
    if (IsSeperate(obbB.axis[1], obbA, obbB)) return false;

    return true;
}

bool BoxCollider::IsSeperate(const Vector2& seperateAxis, const ObbDesc& box1, const ObbDesc& box2)
{
    float d = abs(Vector2::Dot(seperateAxis, box1.pos - box2.pos));

    Vector2 right = box1.axis[0] * box1.halfSize.x;
    Vector2 up = box1.axis[1] * box1.halfSize.y;

    float a = abs(Vector2::Dot(seperateAxis, right)) + abs(Vector2::Dot(seperateAxis, up));

    right = box2.axis[0] * box2.halfSize.x;
    up = box2.axis[1] * box2.halfSize.y;

    float b = abs(Vector2::Dot(seperateAxis, right)) + abs(Vector2::Dot(seperateAxis, up));

    return d > (a + b);
}