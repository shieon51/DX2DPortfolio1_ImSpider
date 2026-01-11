#pragma once

class PlayerSpiderSilk : public SpiderSilk
{
private:
	const float MAX_SILK_LENGTH = 300.0f;
	const float MIN_SILK_LENGTH = 20.0f;
	const float SHOOT_SPEED = 1500.0f;
	const float ADJUST_LENGTH_SPEED = 100.0f;

public:
	PlayerSpiderSilk();
	~PlayerSpiderSilk();

	void Update();
	void Render();

	void SetPlayer(class SpiderPlayer* player) { this->player = player; }

	//player에서 호출
	void SilkShoot(Vector2 playerPos);
	void AdjustSilkLength(bool isUp);
	void Cut();
	Vector2 GetAttachPos() { return attachPos; }
	Vector2 GetFirstPlayerPos() { return firstPos; }
	float GetCurLength();

	//벽돌 충돌에서 호출
	void AttachSilk(Vector2 contectPoint, Vector2 playerPos);

	LineCollider* GetSilkCollider() { return line; }

private:
	void UpdateShooting(); // 발사 중 로직
	void ResetSilk();      // 변수 초기화 로직

private:
	Vector2 shootDir; //거미줄 쏜 방향

	class SpiderPlayer* player;

	bool isShooting = false;
	bool isMaxLength = false;
	bool isHanging = false;
};