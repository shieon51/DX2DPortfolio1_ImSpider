#include "Framework.h"

SpiderJump::SpiderJump(SpiderPlayer* player)
	: SpiderMoveDirection(player)
{
	LoadClip("Textures/Spider/Action/", "Player_Jump.xml", true);
}

// 1. Start 함수 구현 (상태가 JUMP로 바뀌는 순간 딱 1번 실행)
void SpiderJump::Start()
{
    Action::Start(); // 부모(애니메이션 초기화 등) 실행

    // [수정] 조건문 없이 즉시 위로 쏘아 올림
    // 이미 SpiderPlayer가 "뛰어!"라고 명령해서 이 상태로 왔기 때문
    player->SetVelocityY(JUMP_POWER);
}

void SpiderJump::Update()
{
    //점프
    //if (jumpCount <= MAX_JUMP_COUNT && KEY->Down('W'))
    //{
    //    player->SetVelocityY(JUMP_POWER);
    //    jumpCount++;
    //}

    if (player->GetVelocity().y <= 0) //점프 후 최고점에 도달해 아래로 떨어지는 순간이면
    {
        player->Drop(); //드롭 상태로
    }

    SpiderMoveDirection::Update();
}