#pragma once

class GameMath
{
public:
    enum class Direction
    {
        UP, DOWN, LEFT, RIGHT, NONE
    };

    static int Random(const int& min, const int& max);
    static float Random(const float& min, const float& max);

    static bool NearlyEqual(const float& a, const float& b)
    {
        return abs(a - b) < FLT_EPSILON;
    }

    template <typename T>
    static T Lerp(const T& start, const T& end, float t);

    static float Clamp(const float& min, const float& max, const float& value)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;

        return value;
    }
};

template<typename T>
inline T GameMath::Lerp(const T& start, const T& end, float t)
{
    t = Clamp(0.0f, 1.0f, t);

    return start + (end - start) * t;
}
