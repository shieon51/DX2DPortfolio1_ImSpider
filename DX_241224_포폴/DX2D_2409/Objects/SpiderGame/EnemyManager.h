#pragma once

class EnemyManager : public Singleton<EnemyManager>
{
private:
	const UINT SIZE = 20;
public:
	EnemyManager();
	~EnemyManager();

	void Render();
	void Update();

	void SetSpiderPlayer(SpiderPlayer* player) { this->player = player; }
	void SetTileSize(Vector2 size) { tileSize = size; }
	Vector2 GetTileSize() { return tileSize; }

	void AddSpiderEnemy(Vector2 pos, int tileType);
	void TurnOnEnemyActivity();
	void TurnOffEnemyActivity();

	void ClearEnemys();

	//게임 중일 때
	void CheckPlayerCollision(EnemySpider* enemy);

	//MapTileEditer에서 호출
	void CollisionMapAndSilkCheck(Collider* collider);

private:
	vector<EnemySpider*> spiderEnemys;
	SpiderPlayer* player;

	Vector2 tileSize;
};