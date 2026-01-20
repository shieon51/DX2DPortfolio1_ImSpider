#include "Framework.h"

BasicTile::BasicTile(Vector2 size, InstanceData* data)
	: EditTileObject(size, data)
{
}

BasicTile::~BasicTile()
{
}

void BasicTile::CollisionCheck()
{
	Vector2 overlap;

	if (IsCollision(player, &overlap))
	{
		if (overlap.x < overlap.y)//좌우 충돌
		{
			if (GetGlobalPos().x > player->GetGlobalPos().x)
			{
				player->Translate(Vector2::Left() * overlap.x);
			}
			else
			{
				player->Translate(Vector2::Right() * overlap.x);
			}

			//x방향 벨로시티 없애기
			player->SetVelocityX(0.0f);
		}
		else//상하충돌
		{
			if (GetGlobalPos().y > player->GetGlobalPos().y)
			{
				player->Translate(Vector2::Down() * overlap.y);
				player->SetVelocityY(0.0f);
			}
			else
			{
				player->Translate(Vector2::Up() * overlap.y);

				if (player->IsHanging()) //매달린 상태일 경우
				{
					player->SetVelocityX(0.0f); 
				}
				else
				{
					player->Land();
					player->SetLastLandCollisionTilePos(GetGlobalPos());
				}
			}

		}
	}

	//만약 마지막으로 충돌한 바닥타일을 일정거리 이상 벗어나면 -> 절벽 위치인지 확인하기
	if (Vector2::Distance(Vector2(player->GetGlobalPos().x, 0), 
		Vector2(player->GetLastLandCollisionTilePos().x, 0))
				> player->HalfSize().x + HalfSize().x)
	{
		player->GetOffCliff();
	}
}

void BasicTile::SilkCollision(const Vector2& overlap)
{
	if (tileType == SpiderGame::TileType::RAGULAR) //일반 지형만 
	{
		player->GetSpiderSilk()->AttachSilk(overlap, player->GetGlobalPos());
		player->Hang(); //매달리기
	}
}
