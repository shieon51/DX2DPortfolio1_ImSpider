#pragma once

class SpiderJump : public SpiderMoveDirection
{
private:
	const float JUMP_POWER = 500.0f;
	const UINT MAX_JUMP_COUNT = 1;

public:
	SpiderJump(SpiderPlayer* player);

	void Update() override;

	void ResetJumpCount() { jumpCount = MAX_JUMP_COUNT; } //점프횟수 초기화. 바닥에 도달했을 시 호출
	
private:
	int jumpCount = 0;
};