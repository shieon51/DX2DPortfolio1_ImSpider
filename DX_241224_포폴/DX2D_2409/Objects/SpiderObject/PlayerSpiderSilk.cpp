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
		if (!isMaxLength)
		{
			size.x += SHOOT_SPEED * DELTA; //끝 지점
			Translate(GetRight() * SHOOT_SPEED * DELTA); //시작점을 옮기기
			UpdateSilk();

			if (size.x >= MAX_SILK_LENGTH)
				isMaxLength = true;
		}
		else
		{
			size.x -= SHOOT_SPEED * DELTA;
			Translate(GetLeft() * SHOOT_SPEED * DELTA); //시작점을 옮기기
			UpdateSilk();

			if (size.x <= 0) //애니메이션 끝나면 초기화
			{
				pos = Vector2();
				size.x = 0;
				isShooting = false;
				isMaxLength = false;
			}
		}
	}

	if (isHanging)
	{
		//++ 흔들리면 실시간으로 거미줄 각도 조절하기 (줄 길이 고정)
		Vector2 curLineDir = attachPos - player->GetGlobalPos(); //현재 플레이어로부터 부착위치 방향(각도)
		SetRot(0, 0, curLineDir.Angle());
		//player->SetPos(line->End());
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

	//(현재 쏘는 중이었다면 다시 초기위치로 - 예외처리)
	pos = Vector2();
	size.x = 0;
	UpdateSilk();

	//현재 마우스 방향 정보
	shootDir = (mousePos - (playerPos - CAM->GetGlobalPos())).Normalized();
	isShooting = true;

	//현재 방향으로 회전시키기
	SetRot(0, 0, shootDir.Angle());

	pos = Vector2();
}

void PlayerSpiderSilk::AdjustSilkLength(bool isUp) //++나중에 예외처리 (velo.x 갱신 부분)
{
	if (isUp && size.x <= MIN_SILK_LENGTH) return;
	if (!isUp && size.x >= firstSilkLength) return;

	int dir = isUp ? -1 : 1;

	size.x += dir * ADJUST_LENGTH_SPEED * DELTA;
	UpdateSilk();

	firstPos = attachPos + (-1) * shootDir.Normalized() * size.x; //최고점 위치 이동 -> 흔들리는 주기 변경됨
	
}

void PlayerSpiderSilk::Cut()
{
	isHanging = false;
	isShooting = false;

	player->SetParent(nullptr);

	pos = Vector2();
	size.x = 0;
	UpdateSilk();
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
	firstSilkLength = Vector2::Distance(attachPos, firstPos);
	
	//줄 길이 재보정 
	// ++(몇 픽셀 차이로 줄 끝 위치와 player 위치의 차이가 발생해 입력한 예외처리. 좀 더 나은 방식 고민할 것)
	size.x = firstSilkLength;
	UpdateSilk();
	player->SetPos(line->End());

	SetParent(nullptr); //부모(거미) 해제
	pos = attachPos;

	isShooting = false;
	isHanging = true;
}
