#include "Framework.h"

SpiderPhysics::SpiderPhysics()
{
}

SpiderPhysics::~SpiderPhysics()
{
}

void SpiderPhysics::Update()
{
	
	if (!isSpiderPhysicsOn) //hang 상태가 아닌 경우엔 Rigidbody의 물리법칙을 따르도록
	{
		RigidbodyObject::Update();
	}
	else //hang 상태의 경우
	{
		pos += velocity * DELTA;
		Vector2 circleDir = (pos - attachPos).Normalized();
		pos.y = (attachPos + circleDir * curLength).y; // y 위치 

		UpdateWorld();
	}

}

void SpiderPhysics::PendulumMove()
{
	//현재 장력 방향
	Vector2 curTensionDir = attachPos - pos;
	
	//거미의 현재 진행 방향 (장력 + 중력) : 장력과 중력의 크기는 같아야 함.
	Vector2 curHangDir = curTensionDir + Vector2::Down() * curLength;
	
	hangMoveDir = abs(curHangDir.x) < 0.1f ?
		GetDirInLowestPoint() : GetDirGeneralPoint(curHangDir);  //만약 최저점에 도달할 경우 예외처리

	Vector2 accelation = potentialEnergy * hangMoveDir;
	
	//가속 : 진자운동 시작 시 퍼텐셜에너지 x 현재 단진자 방향 벡터
	velocity.x += accelation.x * DELTA;
}

void SpiderPhysics::SetPendulumCondition(Vector2 playerPos, Vector2 attachPos)
{
	firstPlayerPos = playerPos;
	this->attachPos = attachPos;

	//퍼텐셜 에너지 계산 (장력과 중력(장력과 크기 같은 아래방향벡터)의 합)
	Vector2 tensionDir = attachPos - playerPos;
	curLength = tensionDir.Magnitude();
	Vector2 gravityDir = { 0, -curLength };

	potentialEnergy = (tensionDir + gravityDir).Magnitude() * MASS; // 가속의 크기
}

void SpiderPhysics::CheckLeftOrRightDir()
{
	// 현재 진자 시작 위치가 오른쪽인지 왼쪽인지: 왼쪽이면 true : 초기에만 설정
	isLeftToRight = firstPlayerPos.x < attachPos.x; 
}

void SpiderPhysics::RemainVelocityAfterCut()
{
	Vector2 accelation = potentialEnergy * hangMoveDir;

	velocity.y += (velocity.x > 0) ^ (hangMoveDir.x > 0)
		? (-1) * accelation.y  //두 값이 서로 부호가 같으면 -> 아래로 내려가는 중이었음: 아래로 가속	
		: accelation.y;		   //부호가 다르면 -> 위로 올라가는 중이었음: 위로 붕 띄우기 
}

void SpiderPhysics::UpdateVelocityX()
{
	//velocity.x를 갈아끼워야 함.
	//갱신된 길이 기준으로 최고점의 방향벡터 구하기 
	//Vector2 highHangDir = (attachPos - firstPlayerPos) + Vector2::Down() * curLength; 
	////
	//////갱신된 벡터 크기 곱하기
	////Vector2 accelation = potentialEnergy * highHangDir.Normalized();
	//
	////이전 퍼텐셜 에너지  = 
	//float preTotalEnergy = (highHangDir.Normalized() * prePotentialEnergy).y;
	//
	//float preCurTotalEnergy = 
	//
	//
	////
	//Vector2 curTotalEnergy = 
	//
	//
	//현재 구한 accelation의 x를 velocity.x에 대입
	//velocity.x = accelation.x;

}

Vector2 SpiderPhysics::GetDirInLowestPoint()
{
	Vector2 result = isLeftToRight ? Vector2::Right() : Vector2::Left();

	//(해당 함수가 호출된다는 것은 부착지점 기준 진자의 위치가 바뀌었다는 뜻. 방향 바꿔주기)
	if (!alreadyCheck)
	{
		isLeftToRight = !isLeftToRight;
		alreadyCheck = true; //방향 바꿔주기 완료 체크
	}

	return result;
}

Vector2 SpiderPhysics::GetDirGeneralPoint(Vector2 curDir)
{
	if (alreadyCheck) alreadyCheck = false;

	return curDir.Normalized();
}
