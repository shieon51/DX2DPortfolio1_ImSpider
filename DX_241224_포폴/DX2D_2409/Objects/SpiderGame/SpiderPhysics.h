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
	//void CheckLeftOrRightDir();
	//void RemainVelocityAfterCut(); //줄 자른 직후, 적용된 velocity.y 포함하기(예외처리)
	//void SavePrePontentialEnergy() { prePotentialEnergy = potentialEnergy; } //퍼텐셜 상태 저장
	//void UpdateVelocityX(); //줄 길이가 변하면 현재 상태에 대한 velocity.x를 재정의 해야 함

private:
	//Vector2 GetDirInLowestPoint(); //최저점에 대한 예외처리
	//Vector2 GetDirGeneralPoint(Vector2 curDir); //일반적인 위치

protected:
	bool isSpiderPhysicsOn = false; //해당 물리 작용 키기

private:
	float curLength; //현재 줄길이 **

	//Vector2 hangMoveDir; //진자운동시 플레이어의 순간진행방향(단위 벡터)
	//float potentialEnergy; //매달린 순간의 퍼텐셜 에너지(크기)
	//float prePotentialEnergy; //이전 퍼텐셜 에너지(길이 조절 시 비교용)

	Vector2 firstPlayerPos; //초기 플레이어 위치(최고점)
	Vector2 attachPos; //부착 위치
	//bool isLeftToRight; // 진자가 최저점에 도달시 왼->오른쪽 방향으로 갔는가?
	//bool alreadyCheck = false; // 최저점 지나갈 때 예외처리 
};