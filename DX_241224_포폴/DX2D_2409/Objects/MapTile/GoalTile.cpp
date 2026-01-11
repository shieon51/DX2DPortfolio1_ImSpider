#include "Framework.h"

GoalTile::GoalTile(Vector2 size, InstanceData* data)
	: EditTileObject(size, data)
{
}

GoalTile::~GoalTile()
{
}

void GoalTile::CollisionCheck()
{
	Vector2 overlap;

	if (IsCollision(player, &overlap))
	{
		//°ñÀÎ Ã³¸®
		player->GameClear();
	}
}
