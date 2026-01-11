#pragma once

class EnemySpider : public SpiderPhysics
{
protected:
	enum EnemyState //++나중에 거미물리 쪽으로 합치기
	{
		STAND, MOVE, HANG, JUMP
	};

public:
	EnemySpider(Vector2 initPos, int tileType);
	~EnemySpider();

	void Update(); 
	void Render();

	void SwitchOn();
	void SwitchOff();

	int GetTileType() { return tileType; }
	
	virtual void TileCollision(BoxCollider* collider, const Vector2& overlap) = 0;

private:
	virtual void DoActing() = 0; //jump, move, hang
	virtual void CreateActions() = 0;
	virtual void SetAttributes() = 0;

protected:
	bool isEnemyOn = false;

	EnemyState curState;
	map<EnemyState, Action*> enemyActions;

	Vector2 enemyInitialPos;
	int tileType;

	SpiderSilk* silk;
};