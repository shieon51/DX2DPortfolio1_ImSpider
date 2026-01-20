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
		// 1. 속도만큼 이동
		pos += velocity * DELTA;

		Vector2 ropeVec = pos - attachPos;
		Vector2 ropeDir = ropeVec.Normalized();

		// 2. 위치 강제 보정 (줄 길이 유지)
		// 거미가 줄보다 멀어지거나 가까워지면 강제로 줄 길이만큼 떨어진 곳으로 위치시킴
		pos = attachPos + ropeDir * curLength;

		// 3. 속도 벡터 보정 (Radial Velocity Removal)
		// 위치를 강제로 옮겼어도 Velocity가 줄 방향(바깥쪽)을 향하고 있으면
		// 다음 프레임에 또 밖으로 튀어나가려고 해서 불안정하게 흔들릴 수 있음.
		// 따라서 Velocity에서 '줄 방향 성분'을 0으로 잘라버려야 깔끔하게 돎.

		float radialVel = Vector2::Dot(velocity, ropeDir); // 속도 중 '줄 방향 성분' 크기
		velocity -= ropeDir * radialVel; // 순수 접선 속도만 남김

		UpdateWorld();
	}

}

void SpiderPhysics::PendulumMove()
{
	// 1. 현재 줄의 방향 벡터 구하기
	Vector2 ropeVec = pos - attachPos; // 현재 거미 위치 - 부착 위치 => 방향: 부착->거미
	Vector2 ropeDir = ropeVec.Normalized(); // == 줄이 향하는 방향(정규화)

	// 2. 중력 힘 설정 (항상 아래로 작용)
	Vector2 gravity = Vector2(0, -1) * MASS;

	// 3. "접선 방향 힘(Tangential Force)" 구하기
	// 원리: 중력(Gravity)에서 줄을 당기는 힘(Tension)을 제거하면 회전하려는 힘만 남음

	// 3-1. 중력을 줄 방향에 투영(Projection) -> 줄을 당기는 힘 성분
	float tensionScalar = Vector2::Dot(gravity, ropeDir);
	Vector2 tensionForce = ropeDir * tensionScalar;

	// 3-2. 중력 - 줄 당기는 힘 = 순수하게 진자 운동을 시키는 힘 (접선력)
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
}