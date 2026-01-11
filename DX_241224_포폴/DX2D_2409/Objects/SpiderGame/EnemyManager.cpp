#include "Framework.h"

EnemyManager::EnemyManager()
{
	spiderEnemys.reserve(SIZE);
}

EnemyManager::~EnemyManager()
{
	ClearEnemys();
}

void EnemyManager::Render()
{
	for (EnemySpider* enemy : spiderEnemys)
	{
		enemy->Render();
	}
}

void EnemyManager::Update()
{
	for (EnemySpider* enemy : spiderEnemys)
	{
		// (적 움직임 업데이트)
		enemy->Update(); 

		//플레이어 & 적 충돌 -> 리스폰
		CheckPlayerCollision(enemy);
	}
}

void EnemyManager::AddSpiderEnemy(Vector2 pos, int tileType)
{
	if (tileType / 100 == SpiderGame::TileType::JUMP_ENEMY)
	{
		JumpEnemySpider* enemy = new JumpEnemySpider(pos, tileType);
		spiderEnemys.push_back(enemy);
	}
	else if (tileType / 100 == SpiderGame::TileType::MOVE_ENEMY)
	{
		MoveEnemySpider* enemy = new MoveEnemySpider(pos, tileType);
		spiderEnemys.push_back(enemy);
	}
	else if (tileType / 100 == SpiderGame::TileType::HANG_ENEMY)
	{
		HangEnemySpider* enemy = new HangEnemySpider(pos, tileType);
		spiderEnemys.push_back(enemy);
	}

}

void EnemyManager::TurnOnEnemyActivity()
{
	for (EnemySpider* enemy : spiderEnemys)
	{
		if (enemy->GetTileType() / 100 == SpiderGame::TileType::JUMP_ENEMY)
			((JumpEnemySpider*) enemy)->SwitchOn();
		else if (enemy->GetTileType() / 100 == SpiderGame::TileType::MOVE_ENEMY)
			((MoveEnemySpider*)enemy)->SwitchOn();
		else if (enemy->GetTileType() / 100 == SpiderGame::TileType::HANG_ENEMY)
			((HangEnemySpider*)enemy)->SwitchOn();
	}
}

void EnemyManager::TurnOffEnemyActivity()
{
	for (EnemySpider* enemy : spiderEnemys)
	{
		enemy->SwitchOff();
	}
}

void EnemyManager::ClearEnemys()
{
	for (EnemySpider* enemy : spiderEnemys)
		delete enemy;

	spiderEnemys.clear();
}

void EnemyManager::CheckPlayerCollision(EnemySpider* enemy)
{
	if (player->IsCollision(enemy))
	{
		player->Respawn();
	}
}

void EnemyManager::CollisionMapAndSilkCheck(Collider* collider)
{
	// 맵 타일과 충돌할 시: 각 객체별로 적절히 처리
	// 플레이어 줄 & 적 충돌 시: 거미줄 끊기

	Vector2 overlap;

	for (EnemySpider* enemy : spiderEnemys)
	{
		if (enemy->IsCollision(collider, &overlap)) // 적 & 타일 충돌
		{
			enemy->TileCollision((BoxCollider*)collider, overlap);
		}

		if (enemy->IsCollision(player->GetSpiderSilk()->GetSilkCollider(), &overlap)) //적 & 실 충돌
		{
			//플레이어 거미줄 자르기 
			player->WebCut();
		}
	}
}
