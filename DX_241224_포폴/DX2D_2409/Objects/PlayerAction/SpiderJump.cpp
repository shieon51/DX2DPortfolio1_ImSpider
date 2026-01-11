#include "Framework.h"

SpiderJump::SpiderJump(SpiderPlayer* player)
	: SpiderMoveDirection(player)
{
	LoadClip("Textures/Spider/Action/", "Player_Jump.xml", true);
}

void SpiderJump::Update()
{
    //점프
    if (jumpCount <= MAX_JUMP_COUNT && KEY->Down('W'))
    {
        player->SetVelocityY(JUMP_POWER);
        jumpCount++;
    }

    if (player->GetVelocity().y <= 0) //점프 후 최고점에 도달해 아래로 떨어지는 순간이면
    {
        player->Drop(); //드롭 상태로
    }

    SpiderMoveDirection::Update();
}