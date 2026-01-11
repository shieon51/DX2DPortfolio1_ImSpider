#include "Framework.h"

LineCollider::LineCollider(Vector2 startPos, Vector2 endPos)
    : startPos(startPos), endPos(endPos)
{
    tag = "Line";
    type = Type::LINE;

    vector<Vertex>& vertices = mesh->GetVertices();
    vertices.emplace_back(startPos.x, startPos.y);
    vertices.emplace_back(endPos.x, endPos.y);

    vector<UINT>& indices = mesh->GetIndices();
    indices = { 0, 1 };

    mesh->CreateMesh();
}

bool LineCollider::IsPointCollision(const Vector2& point)
{
    return false;
}

bool LineCollider::IsBoxCollision(BoxCollider* box, Vector2* overlap)
{
    BoxCollider::ObbDesc obb;
    box->GetObb(obb);

    Vector2 start = Start();
    Vector2 end = End();
    Vector2 lineDir = end - start;
    Vector2 boxToRay = start - obb.pos;

    float tMin = -1.0f;
    float tMax = 1.0f;

    for (int i = 0; i < 2; i++)
    {
        float axisDotLineDir = Vector2::Dot(obb.axis[i], lineDir);
        float axisDotBoxToLine = Vector2::Dot(obb.axis[i], boxToRay);

        if (GameMath::NearlyEqual(axisDotLineDir, 0.0f))
        {
            if (abs(axisDotBoxToLine) > obb.halfSize[i])
                return false;

            continue;
        }
        
        float t1 = (-axisDotBoxToLine - obb.halfSize[i]) / axisDotLineDir;
        float t2 = (-axisDotBoxToLine + obb.halfSize[i]) / axisDotLineDir;

        if (t1 > t2)
            swap(t1, t2);

        tMin = max(tMin, t1);
        tMax = min(tMax, t2);

        if (tMin > tMax)
            return false;
    }

    if (tMax < 0.0f || tMin > 1.0f)
        return false;

    if (tMin < 0.0f)
        tMin = tMax;

    *overlap = start + lineDir * tMin;

    return true;
}

bool LineCollider::IsCircleCollision(CircleCollider* circle, Vector2* contactPoint)
{
    Vector2 start = Start();
    Vector2 end = End();
    Vector2 center = circle->GetGlobalPos();
    float radius = circle->Radius();

    Vector2 lineDir = end - start;
    Vector2 startToCenter = center - start;

    float a = Vector2::Dot(lineDir, lineDir);
    float b = 2.0f * Vector2::Dot(startToCenter, lineDir);
    float c = Vector2::Dot(startToCenter, startToCenter) - pow(radius, 2);//radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return false;

    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    if (t1 >= 0.0f && t1 <= 1.0f)
    {
        *contactPoint = start + lineDir * t1;
        return true;
    }
    if (t2 >= 0.0f && t2 <= 1.0f)
    {
        *contactPoint = start + lineDir * t2;
        return true;
    }

    return false;
}

bool LineCollider::IsLineCollision(LineCollider* line, Vector2* contactPoint)
{
    Vector2 p1 = Start();
    Vector2 p2 = End();
    Vector2 p3 = line->Start();
    Vector2 p4 = line->End();

    Vector2 d1 = p2 - p1;
    Vector2 d2 = p4 - p3;

    float crossD1D2 = Vector2::Cross(d1, d2);
    if (GameMath::NearlyEqual(crossD1D2, 0.0f))//두 선분이 평행
        return false;

    Vector2 p3p1 = p3 - p1;
    float t1 = Vector2::Cross(p3p1, d2) / crossD1D2;
    float t2 = Vector2::Cross(p3p1, d1) / crossD1D2;

    if (t1 > 0.0f && t1 < 1.0f && t2 > 0.0f && t2 < 1.0f)
    {
        if (contactPoint)
        {
            *contactPoint = Start() + d1 * t1;
        }

        return true;
    }

    return false;
}

void LineCollider::UpdateStartAndEndPos(Vector2 startPos, Vector2 endPos)
{
    this->startPos = startPos;
    this->endPos = endPos;

    vector<Vertex>& vertices = mesh->GetVertices();

    vertices.clear();

    vertices.emplace_back(startPos.x, startPos.y);
    vertices.emplace_back(endPos.x, endPos.y);

    mesh->UpdateVertices();
}
