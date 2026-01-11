#include "Framework.h"

MoveEnemySpider::MoveEnemySpider(Vector2 initPos, int tileType)
	: EnemySpider(initPos, tileType)
{
	CreateActions();
	pos = initPos;
	SetAttributes(); //고유 속성값 저장

	curState = MOVE;
}

void MoveEnemySpider::SwitchOn()
{
	initGravity = true;

	EnemySpider::SwitchOn();
}

void MoveEnemySpider::TileCollision(BoxCollider* collider, const Vector2& overlap)
{
	if (overlap.x > overlap.y)//아래 충돌만 체크
	{
		if (collider->GetGlobalPos().y < GetGlobalPos().y) //바닥 충돌만
		{
			initGravity = false;
			Translate(Vector2::Up() * overlap.y);
			SetVelocityY(0.0f);
		}
	}
}

void MoveEnemySpider::DoActing() //좌우이둥
{
	//초기엔 드롭상태
	if (initGravity)
	{
		Gravity();
	}

	//양옆 move
	pos.x += isRight 
		? ENEMY_MOVE_SPEED * speedMultiplier * DELTA  //오른쪽 이동
		: (-1) * ENEMY_MOVE_SPEED * speedMultiplier * DELTA; //왼쪽 이동

	//방향 바꿔주기
	if (pos.x - HalfSize().x <= leftLimit) //왼쪽 끝
		isRight = true;
	else if (pos.x + HalfSize().x >= rightLimit) //오른쪽 끝
		isRight = false;
	
}

void MoveEnemySpider::CreateActions()
{
	enemyActions[MOVE] = new Action("Textures/Spider/Action/", "MoveEnemy_Move.xml", true);
}

void MoveEnemySpider::SetAttributes()
{
	//**  tileType 정보 예시  **//
	// ex) tileType = 2143  			
	// 21: MoveEnemy
	//  4: 이동 범위(칸 수)
	//  3: 이동 속도 배율 (지정된 속도의 3배 빠르기로 이동)
	int remainNum = tileType % 100;

	rangeTileNum = remainNum / 10;
	speedMultiplier = remainNum % 10;


	//이동 범위 제한값 설정
	tileSize = EnemyManager::Get()->GetTileSize();
	leftLimit = enemyInitialPos.x - (tileSize.x * 0.5f);
	rightLimit = leftLimit + (tileSize.x * rangeTileNum);
}


