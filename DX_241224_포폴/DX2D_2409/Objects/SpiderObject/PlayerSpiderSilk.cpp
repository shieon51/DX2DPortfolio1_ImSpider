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
	if (isShooting) //쏜 경우 
	{
		UpdateShooting();
	}

	if (isHanging)
	{
		Vector2 curLineDir = attachPos - player->GetGlobalPos(); //현재 플레이어로부터 부착위치 방향(각도)
		SetRot(0, 0, curLineDir.Angle());
		//player->SetPos(line->End());

		// 줄 길이 업데이트
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

	ResetSilk();
	isShooting = true;

	//현재 마우스 방향 정보
	Vector2 dist = mousePos - (playerPos - CAM->GetGlobalPos());
	shootDir = dist.Normalized();

	SetRot(0, 0, shootDir.Angle() + XM_PI);

	pos = Vector2();
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
	// 줄 길이 재보정 
	firstSilkLength = Vector2::Distance(attachPos, firstPos);
	size.x = firstSilkLength;
	
	//player->SetPos(line->End());

	SetParent(nullptr); //부모(거미) 해제
	pos = attachPos;

	// 매달리기 상태의 각도로 바로 갱신해줘야 함
	Vector2 dir = attachPos - playerPos;
	SetRot(0, 0, dir.Angle());

	UpdateSilk();

	isShooting = false;
	isHanging = true;
}

// [리팩토링] 발사 로직 분리
void PlayerSpiderSilk::UpdateShooting()
{
	pos = Vector2(0, 0);

	// 1. 발사 (나가는 중)
	if (!isMaxLength)
	{
		// 길이만 늘리기 (오른쪽으로 쭉 뻗음)
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
		// 길이만 줄이기 (오른쪽 끝에서부터 줄어듦)
		size.x -= SHOOT_SPEED * DELTA;
		UpdateSilk();

		// 길이가 0이 되면 (내 몸에 다 돌아오면) 끝
		if (size.x <= 0)
		{
			size.x = 0;
			ResetSilk(); // 종료 및 초기화
		}
	}
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
