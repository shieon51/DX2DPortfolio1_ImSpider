#include "Framework.h"

SpiderMove::SpiderMove(SpiderPlayer* player)
	: SpiderMoveDirection(player)
{
	LoadClip("Textures/Spider/Action/", "Player_Move.xml", true);
}
