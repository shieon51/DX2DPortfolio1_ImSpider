#pragma once

class SpiderPhysics : public RigidbodyObject //매달린 경우의 물리 추가
{
private: 
	const float MASS = 4200.0f; //진자의 흔들림 정도 (질량에 해당)

public:
	SpiderPhysics();
	~SpiderPhysics();

	void Update();

	//진자 운동 함수
	void PendulumMove();

	//player에서 호출 
	void SetPendulumCondition(Vector2 playerPos, Vector2 attachPos);


protected:
	bool isSpiderPhysicsOn = false; //해당 물리 작용 키기

private:
	float curLength; //현재 줄길이 

	Vector2 firstPlayerPos; //초기 플레이어 위치(최고점)
	Vector2 attachPos; //부착 위치
};