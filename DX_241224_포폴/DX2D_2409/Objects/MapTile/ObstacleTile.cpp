#include "Framework.h"

ObstacleTile::ObstacleTile(Vector2 size, InstanceData* data)
	: EditTileObject(size, data)
{
}

ObstacleTile::~ObstacleTile()
{
}

void ObstacleTile::CollisionCheck()
{
	Vector2 overlap;

	if (IsCollision(player, &overlap))
	{
		//목숨 차감 및 초기 위치로 리스폰
		player->Respawn();
	}
}

void ObstacleTile::SilkCollision(const Vector2& overlap)
{
	if (tileType == SpiderGame::TileType::COBWEB) //거미집 장애물 타일만 부착 가능
	{
		player->GetSpiderSilk()->AttachSilk(overlap, player->GetGlobalPos());
		player->Hang(); //매달리기
	}
	else //나머지는 컷
	{
		player->WebCut();
	}
}
