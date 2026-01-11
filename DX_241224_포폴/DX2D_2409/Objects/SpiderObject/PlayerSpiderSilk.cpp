#include "Framework.h"

PlayerSpiderSilk::PlayerSpiderSilk()
{
	SetTag("PlayerSpiderSilk");

	UpdateSilk();
}

PlayerSpiderSilk::~PlayerSpiderSilk()
{
}

void PlayerSpiderSilk::Update()
{
	if (isShooting) //쏜 경우 ++ 나중에 함수로 정리
	{
		UpdateShooting();
	}

	if (isHanging)
	{
		//++ 흔들리면 실시간으로 거미줄 각도 조절하기 (줄 길이 고정)
		Vector2 curLineDir = attachPos - player->GetGlobalPos(); //현재 플레이어로부터 부착위치 방향(각도)
		SetRot(0, 0, curLineDir.Angle());
		//player->SetPos(line->End());

		// 줄 길이 업데이트 (혹시나 오차가 생기면 시각적으로 보정)
		// size.x = curLineDir.Magnitude(); // 필요하다면 주석 해제하여 사용
		UpdateSilk();
	}

	UpdateWorld();
	line->UpdateWorld();
}

void PlayerSpiderSilk::Render()
{
	Rope::Render();
	line->Render();
}

void PlayerSpiderSilk::SilkShoot(Vector2 playerPos)
{
	Audio::Get()->Play("SilkShoot");

	////(현재 쏘는 중이었다면 다시 초기위치로 - 예외처리)
	//pos = Vector2();
	//size.x = 0;
	//UpdateSilk();

	// [수정 1] 버그 수정: isMaxLength를 반드시 false로 초기화해야 함!
	ResetSilk();
	isShooting = true;

	//현재 마우스 방향 정보
	Vector2 dist = mousePos - (playerPos - CAM->GetGlobalPos());
	shootDir = dist.Normalized();
	//isShooting = true;

	//현재 방향으로 회전시키기
	//SetRot(0, 0, shootDir.Angle());
	
	// 2. [핵심 수정] 
	// 이미지가 왼쪽을 보고 있다면, 180도(XM_PI)를 더해서 강제로 뒤집어줍니다.
	// 이제 거미줄은 거미 몸(Pivot)을 기준으로 마우스 방향으로 뻗어나갑니다.
	SetRot(0, 0, shootDir.Angle() + XM_PI);

	pos = Vector2();
}

void PlayerSpiderSilk::AdjustSilkLength(bool isUp) //++나중에 예외처리 (velo.x 갱신 부분)
{
	if (isUp && size.x <= MIN_SILK_LENGTH) return;
	if (!isUp && size.x >= firstSilkLength) return;

	float amount = ADJUST_LENGTH_SPEED * DELTA;
	if (isUp) amount *= -1.0f; // 줄어듦

	size.x += amount;
	UpdateSilk();

	// [중요] 줄 길이가 변하면 매달린 상태에서 플레이어의 물리적 위치(firstPos)도 당겨지거나 늘어남
	// 이 부분은 물리 연산(SpiderPlayer) 쪽에서 처리하는 것이 맞으나,
	// 여기서 위치를 강제로 잡아야 한다면 아래 로직 유지.
	// 하지만 보통은 size.x만 바꾸고 Player쪽에서 Pendulum 길이를 갱신하는 게 자연스러움.

	//firstPos = attachPos + (-1) * shootDir.Normalized() * size.x; //최고점 위치 이동 -> 흔들리는 주기 변경됨
	
}

void PlayerSpiderSilk::Cut()
{
	/*isHanging = false;
	isShooting = false;*/

	ResetSilk();
	player->SetParent(nullptr);

	/*pos = Vector2();
	size.x = 0;
	UpdateSilk();*/
}

float PlayerSpiderSilk::GetCurLength()
{
	return size.x;
}

void PlayerSpiderSilk::AttachSilk(Vector2 contectPoint, Vector2 playerPos)
{
	Audio::Get()->Stop("SilkShoot");
	Audio::Get()->Play("SilkAttach");

	//현재 부착된 위치 & 부착시 플레이어 위치 저장 (world 기준)
	attachPos = contectPoint;
	firstPos = playerPos;

	// 부착된 지점까지의 거리로 줄 길이 확정 
	// 줄 길이 재보정 ++(몇 픽셀 차이로 줄 끝 위치와 player 위치의 차이가 발생해 입력한 예외처리. 좀 더 나은 방식 고민할 것)
	firstSilkLength = Vector2::Distance(attachPos, firstPos);
	size.x = firstSilkLength;
	
	//player->SetPos(line->End());

	SetParent(nullptr); //부모(거미) 해제
	pos = attachPos;

	// [추가] 중요! 매달리기 상태의 각도로 즉시 갱신해줘야 함.
	// (이게 없으면 쏘던 각도 그대로 유지돼서 줄이 엉뚱한 곳을 가리킴)
	Vector2 dir = attachPos - playerPos;
	SetRot(0, 0, dir.Angle());

	UpdateSilk();

	isShooting = false;
	isHanging = true;
}

// [리팩토링] 발사 로직 분리
void PlayerSpiderSilk::UpdateShooting()
{
	// [수정 2] Translate 삭제. 
	// 피벗이 좌측 중앙이므로 위치 이동 없이 길이(size.x)만 늘렸다 줄이면 됨.

	pos = Vector2(0, 0);

	// 1. 발사 (나가는 중)
	if (!isMaxLength)
	{
		// 길이만 늘립니다 (오른쪽으로 쭉 뻗음)
		size.x += SHOOT_SPEED * DELTA;
		UpdateSilk();

		// 최대 길이에 도달하면 '복귀 모드'로 전환
		if (size.x >= MAX_SILK_LENGTH)
		{
			isMaxLength = true;
			size.x = MAX_SILK_LENGTH; // 길이 초과 방지 보정
		}
	}
	// 2. 복귀 (돌아오는 중)
	else
	{
		// 길이만 줄입니다 (오른쪽 끝에서부터 줄어듦)
		size.x -= SHOOT_SPEED * DELTA;
		UpdateSilk();

		// 길이가 0이 되면 (내 몸에 다 돌아오면) 끝!
		if (size.x <= 0)
		{
			size.x = 0;
			ResetSilk(); // 종료 및 초기화
		}
	}

	//if (!isMaxLength)
		//{
		//	size.x += SHOOT_SPEED * DELTA; //끝 지점
		//	Translate(GetRight() * SHOOT_SPEED * DELTA); //시작점을 옮기기
		//	UpdateSilk();

		//	if (size.x >= MAX_SILK_LENGTH)
		//		isMaxLength = true;
		//}
		//else
		//{
		//	size.x -= SHOOT_SPEED * DELTA;
		//	Translate(GetLeft() * SHOOT_SPEED * DELTA); //시작점을 옮기기
		//	UpdateSilk();

		//	if (size.x <= 0) //애니메이션 끝나면 초기화
		//	{
		//		pos = Vector2();
		//		size.x = 0;
		//		isShooting = false;
		//		isMaxLength = false;
		//	}
		//}
}

void PlayerSpiderSilk::ResetSilk()
{
	isShooting = false;
	isMaxLength = false;
	isHanging = false;

	pos = Vector2(0, 0);
	size.x = 0;
	UpdateSilk();
}
