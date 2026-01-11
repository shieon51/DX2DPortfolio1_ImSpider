#pragma once

class JumpEnemySpider : public EnemySpider
{
private:
	const float ENEMY_JUMP_POWER = 10.0f;

public:
	JumpEnemySpider(Vector2 initPos, int tileType);
	~JumpEnemySpider();

	// EnemySpider을(를) 통해 상속됨
	void TileCollision(BoxCollider* collider, const Vector2& overlap) override;

private:
	void DoActing() override;
	void CreateActions() override;
	void SetAttributes() override;

private:
	//점프 높이 배율
	int jumpMultiplier;

	//바닥에 닿았는가?
	bool isLand = false; 

};