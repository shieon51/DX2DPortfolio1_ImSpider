#include "Framework.h"

HangEnemySpider::HangEnemySpider(Vector2 initPos, int tileType)
	: EnemySpider(initPos, tileType)
{
	silk = new SpiderSilk();

	CreateActions();
	pos = initPos;
	SetAttributes(); //고유 속성값 저장

	curState = HANG;
}

void HangEnemySpider::SwitchOn()
{
	SetPos(firstPos);

	EnemySpider::SwitchOn();
}

void HangEnemySpider::TileCollision(BoxCollider* collider, const Vector2& overlap)
{
	return;
}

void HangEnemySpider::DoActing()
{
	silk->Update();
}

void HangEnemySpider::CreateActions()
{
	enemyActions[HANG] = new Action("Textures/Spider/Action/", "HangEnemy_Hang.xml", false);
}

void HangEnemySpider::SetAttributes()
{
	//**  tileType 정보 예시  **//
	// ex) tileType = 2213
	// 22: HangEnemy
	//  1: 줄 길이(칸 수)
	//  3: 흔들리는 각도 배율 (0은 흔들리지 않음)

	int remainNum = tileType % 100;

	silkLengthNum = remainNum / 10;
	hangAngleMultiplier = remainNum % 10;

	isMoving = (bool) hangAngleMultiplier;
	tileSize = EnemyManager::Get()->GetTileSize();

	//if (isMoving)
	//{
	//	Vector2 attachPos = pos + Vector2(0, tileSize.y * silkLengthNum - tileSize.y * 0.5f);
	//	Vector2 firstPos = ;
	//	SetPendulumCondition(firstPos, );
	//}

	firstPos = pos + Vector2(0, (-1) * tileSize.y * silkLengthNum);
	Vector2 attachPos = pos + Vector2(0, tileSize.y * 0.5f);

	silk->SetInitPos(attachPos, firstPos);
}
