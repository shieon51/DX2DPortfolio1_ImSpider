#pragma once

class HangEnemySpider : public EnemySpider
{
private:
	const float ANGLE_CONST = 10.0f;

public:
	HangEnemySpider(Vector2 initPos, int tileType);
	~HangEnemySpider();

	void SwitchOn();

	void Render();

	// EnemySpider을(를) 통해 상속됨
	void TileCollision(BoxCollider* collider, const Vector2& overlap) override;

private:
	void DoActing() override;
	void CreateActions() override;
	void SetAttributes() override;

private:
	//줄 길이(칸 수 만큼)
	int silkLengthNum;
	//흔들리는 각도 배율 (0은 가만히, 1 이상은 흔들림)
	int hangAngleMultiplier;

	//게임 진행 중 거미 위치
	Vector2 firstPos;
	//최고점일 때, 부착 위치로부터 x축 방향으로 떨어진 정도
	float topAnglePosX;

	Vector2 tileSize;
	bool isMoving = false;
};