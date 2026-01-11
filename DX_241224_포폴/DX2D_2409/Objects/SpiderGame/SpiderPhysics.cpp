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
		// 1. 일단 속도만큼 이동
		pos += velocity * DELTA;

		//Vector2 circleDir = (pos - attachPos).Normalized();
		//pos.y = (attachPos + circleDir * curLength).y; // y 위치 


		Vector2 ropeVec = pos - attachPos;
		Vector2 ropeDir = ropeVec.Normalized();

		// 2. 위치 강제 보정 (줄 길이 유지)
		// 거미가 줄보다 멀어지거나 가까워지면 강제로 줄 길이만큼 떨어진 곳으로 위치시킴
		pos = attachPos + ropeDir * curLength;

		// 3. [핵심] 속도 벡터 보정 (Radial Velocity Removal)
		// 설명: 위치를 강제로 옮겼어도, '속도(Velocity)'가 줄 방향(바깥쪽)을 향하고 있으면
		// 다음 프레임에 또 밖으로 튀어나가려고 해서 덜덜거리거나 에너지가 폭주합니다.
		// 따라서 속도 중에서 "줄 방향 성분"을 0으로 싹둑 잘라버려야 깔끔하게 돕니다.

		float radialVel = Vector2::Dot(velocity, ropeDir); // 속도 중 줄 방향 성분 크기
		velocity -= ropeDir * radialVel; // 속도에서 그 성분을 뺌 (순수 접선 속도만 남김)

		UpdateWorld();
	}

}

void SpiderPhysics::PendulumMove()
{
	////현재 장력 방향
	//Vector2 curTensionDir = attachPos - pos;
	//
	////거미의 현재 진행 방향 (장력 + 중력) : 장력과 중력의 크기는 같아야 함.
	//Vector2 curHangDir = curTensionDir + Vector2::Down() * curLength;
	//
	//hangMoveDir = abs(curHangDir.x) < 0.1f ?
	//	GetDirInLowestPoint() : GetDirGeneralPoint(curHangDir);  //만약 최저점에 도달할 경우 예외처리

	//Vector2 accelation = potentialEnergy * hangMoveDir;
	//
	////가속 : 진자운동 시작 시 퍼텐셜에너지 x 현재 단진자 방향 벡터
	//velocity.x += accelation.x * DELTA;


	// 1. 현재 줄의 방향 벡터 구하기
	Vector2 ropeVec = pos - attachPos;
	Vector2 ropeDir = ropeVec.Normalized();

	// 2. 중력 힘 설정 (항상 아래로 작용)
	Vector2 gravity = Vector2(0, -1) * MASS;

	// 3. [핵심] "접선 방향 힘(Tangential Force)" 구하기
	// 원리: 중력(Gravity)에서 줄을 당기는 힘(Tension)을 제거하면, 회전하려는 힘만 남음

	// 3-1. 중력을 줄 방향에 투영(Projection) -> 줄을 당기는 힘 성분
	float tensionScalar = Vector2::Dot(gravity, ropeDir);
	Vector2 tensionForce = ropeDir * tensionScalar;

	// 3-2. 중력 - 줄 당기는 힘 = 순수하게 진자 운동을 시키는 힘
	Vector2 tangentialForce = gravity - tensionForce;

	// 4. 가속도 적용 (F = ma)
	velocity += tangentialForce * DELTA;
}

void SpiderPhysics::SetPendulumCondition(Vector2 playerPos, Vector2 attachPos)
{
	this->firstPlayerPos = playerPos;
	this->attachPos = attachPos;

	// 줄 길이 저장
	this->curLength = Vector2::Distance(playerPos, attachPos);

	////퍼텐셜 에너지 계산 (장력과 중력(장력과 크기 같은 아래방향벡터)의 합)
	//Vector2 tensionDir = attachPos - playerPos;
	//curLength = tensionDir.Magnitude();
	//Vector2 gravityDir = { 0, -curLength };

	//potentialEnergy = (tensionDir + gravityDir).Magnitude() * MASS; // 가속의 크기
}

//void SpiderPhysics::CheckLeftOrRightDir()
//{
//	// 현재 진자 시작 위치가 오른쪽인지 왼쪽인지: 왼쪽이면 true : 초기에만 설정
//	isLeftToRight = firstPlayerPos.x < attachPos.x; 
//}

void SpiderPhysics::RemainVelocityAfterCut()
{
	//Vector2 accelation = potentialEnergy * hangMoveDir;

	//velocity.y += (velocity.x > 0) ^ (hangMoveDir.x > 0)
	//	? (-1) * accelation.y  //두 값이 서로 부호가 같으면 -> 아래로 내려가는 중이었음: 아래로 가속	
	//	: accelation.y;		   //부호가 다르면 -> 위로 올라가는 중이었음: 위로 붕 띄우기 
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

//Vector2 SpiderPhysics::GetDirInLowestPoint()
//{
//	Vector2 result = isLeftToRight ? Vector2::Right() : Vector2::Left();
//
//	//(해당 함수가 호출된다는 것은 부착지점 기준 진자의 위치가 바뀌었다는 뜻. 방향 바꿔주기)
//	if (!alreadyCheck)
//	{
//		isLeftToRight = !isLeftToRight;
//		alreadyCheck = true; //방향 바꿔주기 완료 체크
//	}
//
//	return result;
//}

//Vector2 SpiderPhysics::GetDirGeneralPoint(Vector2 curDir)
//{
//	if (alreadyCheck) alreadyCheck = false;
//
//	return curDir.Normalized();
//}
