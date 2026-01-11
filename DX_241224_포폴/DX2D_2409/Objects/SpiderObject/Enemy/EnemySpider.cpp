#include "Framework.h"

EnemySpider::EnemySpider(Vector2 initPos, int tileType)
	: enemyInitialPos(initPos), tileType(tileType)
{
	SetTag("EnemySpider");

	UpdateSize(Vector2(50, 25));

}

EnemySpider::~EnemySpider()
{
	for (pair<EnemyState, Action*> action : enemyActions)
		delete action.second;
}

void EnemySpider::Update()
{
	if (!isEnemyOn) return;

	DoActing();

	enemyActions[curState]->Update();
	SpiderPhysics::Update();
}

void EnemySpider::Render()
{
	if (!isEnemyOn) return;

	if (tileType / 100 == SpiderGame::TileType::HANG_ENEMY)
		silk->Render();

	SetWorld();
	enemyActions[curState]->Render();
	Collider::Render();
}

void EnemySpider::SwitchOn()
{
	isEnemyOn = true;
}

void EnemySpider::SwitchOff()
{
	isEnemyOn = false;
	pos = enemyInitialPos; //초기 위치로 돌아가도록
}


