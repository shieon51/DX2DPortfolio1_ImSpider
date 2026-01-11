#include "Framework.h"

SpiderDrop::SpiderDrop(SpiderPlayer* player)
	:SpiderMoveDirection(player)
{
	LoadClip("Textures/Spider/Action/", "Player_Fall.xml", true);
}
