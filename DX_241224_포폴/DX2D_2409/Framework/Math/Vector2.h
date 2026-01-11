#pragma once
#include "Framework/Math/GameMath.h"

struct Vector2 : public Float2
{
    Vector2(float x = 0.0f, float y = 0.0f)
        : Float2(x, y)
    {
    }
    Vector2(Float2 value) : Float2(value)
    {
    }

    Vector2 operator+(const Vector2& v) const
    {
        return Vector2(x + v.x, y + v.y);
    }
    Vector2 operator-(const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }
    Vector2 operator*(const Vector2& v) const
    {
        return Vector2(x * v.x, y * v.y);
    }
    Vector2 operator/(const Vector2& v) const
    {
        return Vector2(x / v.x, y / v.y);
    }
    Vector2 operator*(const float& s) const
    {
        return Vector2(x * s, y * s);
    }
    Vector2 operator/(const float& s) const
    {
        return Vector2(x / s, y / s);
    }

    bool operator==(const Vector2& v) //Ãß°¡
    {
        return GameMath::NearlyEqual(x, v.x) 
            && GameMath::NearlyEqual(y, v.y);
    }
    bool operator!=(const Vector2& v)
    {
        return x != v.x || y != v.y;
    }

    void operator+=(const Vector2& v)
    {
        x += v.x;
        y += v.y;
    }
    void operator-=(const Vector2& v)
    {
        x -= v.x;
        y -= v.y;
    }
    void operator*=(const float& s)
    {
        x *= s;
        y *= s;
    }
    void operator/=(const float& s)
    {
        x /= s;
        y /= s;
    }
    Vector2 operator*(const Matrix& m) const
    {
        XMVECTOR coord = XMLoadFloat2(this);
        coord = XMVector2TransformCoord(coord, m);
        
        Vector2 result;
        XMStoreFloat2(&result, coord);

        return result;
    }
    float operator[](const int& index) const
    {
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        }

        return 0.0f;
    }

    friend Vector2 operator*(const float& s, const Vector2& v)
    {
        return Vector2(s * v.x, s * v.y);
    }
    friend Vector2 operator/(const float& s, const Vector2 & v)
    {
        return Vector2(s / v.x, s / v.y);
    }

    float SqrMagnitude() const
    {
        return x * x + y * y;
    }

    float Magnitude() const
    {
        return sqrt(SqrMagnitude());
    }

    float Angle() const
    {
        return atan2(y, x);
    }

    void Normalize()
    {
        float magnitude = Magnitude();

        x /= magnitude;
        y /= magnitude;
    }

    Vector2 Normalized() const
    {
        float magnitude = Magnitude();

        return Vector2(x / magnitude, y / magnitude);
    }

    static float Distance(const Vector2& v1, const Vector2& v2)
    {
        return (v2 - v1).Magnitude();
    }

    static float Cross(const Vector2& v1, const Vector2& v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    static float Dot(const Vector2& v1, const Vector2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    static Vector2 Up() { return Vector2(0, 1); }
    static Vector2 Down() { return Vector2(0, -1); }
    static Vector2 Right() { return Vector2(1, 0); }
    static Vector2 Left() { return Vector2(-1, 0); }
};