#include "Framework.h"

SpiderMoveDirection::SpiderMoveDirection(SpiderPlayer* player)
	: SpiderAction(player)
{
}

void SpiderMoveDirection::Update()
{
    //날아가는 상태가 아닌 경우만 좌우 움직이기 가능
    if (player->GetCurSituation() != SpiderPlayer::GameProgressState::FLY_AWAY) 
    {
        if (KEY->Press('A'))
            player->HorizontalMove(false);
        if (KEY->Press('D'))
            player->HorizontalMove(true);
    }

    Action::Update();
}
