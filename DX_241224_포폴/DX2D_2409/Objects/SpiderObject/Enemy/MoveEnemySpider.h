#pragma once

class MoveEnemySpider : public EnemySpider
{
private:
	const float ENEMY_MOVE_SPEED = 50.0f;

public:
	MoveEnemySpider(Vector2 initPos, int tileType);
	~MoveEnemySpider();

	void SwitchOn();

	// EnemySpider을(를) 통해 상속됨
	void TileCollision(BoxCollider* collider, const Vector2& overlap) override;

private:
	void DoActing() override;
	void CreateActions() override;
	void SetAttributes() override;

private:
	//이동 범위 칸 수(타일 맵의 칸 수 저장)
	int rangeTileNum; 

	//이동 속도의 배율
	int speedMultiplier;

	//현재 이동 방향
	bool isRight = true;

	Vector2 tileSize;
	bool initGravity = true;

	//(x 범위) 이동 범위 제한
	float leftLimit;
	float rightLimit;
};