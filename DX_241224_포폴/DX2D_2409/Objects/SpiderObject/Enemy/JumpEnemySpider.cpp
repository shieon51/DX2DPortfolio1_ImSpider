#include "Framework.h"

JumpEnemySpider::JumpEnemySpider(Vector2 initPos, int tileType)
	: EnemySpider(initPos, tileType)
{
	CreateActions();
	pos = initPos;
	SetAttributes(); //고유 속성값 저장

	curState = STAND;
}

void JumpEnemySpider::TileCollision(BoxCollider* collider, const Vector2& overlap)
{
	if (overlap.x > overlap.y)//아래 충돌만 체크
	{
		if (collider->GetGlobalPos().y < GetGlobalPos().y) //바닥 충돌만
		{
			isLand = true;
			curState = STAND;
			Translate(Vector2::Up() * overlap.y);
			SetVelocityY(0.0f);
		}
	}
}

void JumpEnemySpider::DoActing()
{
	if (isLand) //바닥에 닿으면 다시 점프
	{
		//++나중에 점프 텀 줄 수 있으면 주기

		SetVelocityY(ENEMY_JUMP_POWER * jumpMultiplier);
		curState = JUMP;
		isLand = false;
	}
	else //공중에서는
	{
		Gravity();
	}

}

void JumpEnemySpider::CreateActions()
{
	enemyActions[STAND] = new Action("Textures/Spider/Action/", "JumpEnemy_Stand.xml", false);
	enemyActions[JUMP] = new Action("Textures/Spider/Action/", "JumpEnemy_Jump.xml", false);
}

void JumpEnemySpider::SetAttributes()
{
	//**  tileType 정보 예시  **//
	// ex) tileType = 2060
	// 20: JumpEnemy
	// 60: 점프 파워 배율 (지정된 파워(10)의 60배 => 총 점프파워 600)

	int remainNum = tileType % 100;
	jumpMultiplier = remainNum;
}
