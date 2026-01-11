#include "Framework.h"

RigidbodyObject::RigidbodyObject()
{
}

RigidbodyObject::~RigidbodyObject()
{
}

void RigidbodyObject::Update()
{
    if (isRigidbodyLimitOn)
    {
        velocity.x = GameMath::Clamp(-MOVE_SPEED, MOVE_SPEED, velocity.x);
        velocity.x = GameMath::Lerp(velocity.x, 0.0f, DECELELATION * DELTA);
    }
    //velocity.y -= GRAVITY * DELTA;

    pos += velocity * DELTA;

    UpdateWorld();
}

void RigidbodyObject::HorizontalMove(bool isRight)
{
    float accelation = isRight ? ACCELATION : -ACCELATION;

    velocity.x += accelation * DELTA;
}

void RigidbodyObject::Gravity()
{
    velocity.y -= GRAVITY * DELTA;
}
