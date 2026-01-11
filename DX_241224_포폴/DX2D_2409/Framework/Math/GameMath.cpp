#include "Framework.h"
#include "GameMath.h"

//monster = Random(0, monsters.size());

//Max Exclusive
int GameMath::Random(const int& min, const int& max)
{
    return rand() % (max - min) + min;
}

//Max Inclusive
float GameMath::Random(const float& min, const float& max)
{
    float normal = rand() / (float)RAND_MAX;//0~1;

    return min + (max - min) * normal;
}
