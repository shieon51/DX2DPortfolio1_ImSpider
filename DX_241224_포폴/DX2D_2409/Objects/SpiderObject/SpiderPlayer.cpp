#include "SpiderPlayer.h"
#include "Framework.h"

SpiderPlayer::SpiderPlayer()
{
	SetTag("SpiderPlayer");

	//플레이어 콜라이더 사이즈
	UpdateSize(Vector2(50, 25));

	spiderSilk = new PlayerSpiderSilk();
	spiderSilk->SetParent(this);
	spiderSilk->SetPlayer(this); //Hang 상태를 제외하곤 실크는 플레이어를 부모로
	//spiderSilk->SetActive(false);

	life = MAX_LIFE;

	CreateActions();

	SetActionState(STAND);

	curSituation = DEFAULT;
}

SpiderPlayer::~SpiderPlayer()
{
	for (pair<ActionState, Action*> action : spiderActions)
		delete action.second;

	delete spiderSilk;
	delete intValueBuffer;
	delete colorValueBuffer;
}

void SpiderPlayer::Update()
{
	SpiderPhysics::Update();

	// ---------------- [추가/수정] ----------------
	// 1. 상태(State)와 상관없이 W키를 누르면 0.1초 동안 입력 기억 (선입력)
	if (KEY->Down('W'))
	{
		jumpBufferTime = MAX_COYOTE_TIME; // 0.1초(취향에 따라 조절) 동안은 "눌렀다"고 인정해줌
	}

	// 2. 시간 차감 (버퍼 비우기)
	if (jumpBufferTime > 0.0f)
	{
		jumpBufferTime -= DELTA;
	}
	// ---------------------------------------------

	if (curSituation != GOAL_IN)
	{
		Jump();
		Move();
	
		WebShoot();
	}

	WebCutCheck();
	AdjustWebLength();

	Floating(); //중력 작용
	RespawnTimer(); //리스폰 된 직후 깜빡임 처리

	spiderSilk->Update();
	spiderActions[curState]->Update();

	if (UIManager::Get()->GetUIPanel("GameResult")->IsOn())
		UIManager::Get()->GetUIPanel("GameResult")->UpdateWorld();
}

void SpiderPlayer::Render()
{
	intValueBuffer->SetPS(2);
	colorValueBuffer->SetPS(3);

	spiderSilk->Render();

	SetWorld();
	spiderActions[curState]->Render();

	Collider::Render();
}

void SpiderPlayer::Edit()
{
	ImGui::Text("Life: %d", life);

	if (isGameClear)
	{
		ImGui::Text("Goal!");
	}
}

void SpiderPlayer::Land() //에디터에서 일반 벽돌 충돌 시 호출함.
{
	// [추가] 방금 점프해서 위로 올라가는 중이라면(속도가 양수), 
	// 발이 땅에 닿아있더라도 착지 판정을 하지 말고 무시해라!
	if (velocity.y > 0.0f) return;

	if (curState <= MOVE) return;

	if (curSituation == FLY_AWAY) //날아가다 착지한 경우만 (리스폰의 경우는 해당 x)
		curSituation = DEFAULT;

	isRigidbodyLimitOn = true;
	SetActionState(STAND);

	//점프 횟수 초기화
	((SpiderJump*)spiderActions[JUMP])->ResetJumpCount();
}

void SpiderPlayer::Drop() // 임시 코드
{
	if (curState == DROP) return;
	
	SetActionState(DROP);
}

void SpiderPlayer::Floating()
{
	if (curState >= JUMP) //현재 공중에 뜬 상태면
	{
		Gravity();
	}
	else if (curState == HANG)
	{
		PendulumMove(); //pos받을 필요 없음
	}
	else 
	{
		SetVelocityY(0.0f);
	}
}

void SpiderPlayer::RespawnTimer()
{
	if (curSituation != RESPAWNED) return;

	//리스폰이 된 직후에는 일정시간 동안 깜박임
	respawnedTime += DELTA;

	if (respawnedTime >= MAX_RESPAWNED_TIME) //일정 시간 지나면
	{
		curSituation = DEFAULT; //기본 상태로 전환
		respawnedTime = 0;
		intValueBuffer->Get()[SWITCH_SLOT] = OFF;
	}
	else //빨간색 점멸 
	{
		DemageEffect(respawnedTime);
	}
}

void SpiderPlayer::PlayerReset()
{
	//목숨, 골인상태 등 초기화 
	WebCut(); //거미줄 부착 상태였다면 자르기
	UpdateWorld();

	life = MAX_LIFE;
	isGameClear = false;
	curSituation = DEFAULT;

	respawnedTime = 0;
	blinkTime = 0;
	intValueBuffer->Get()[SWITCH_SLOT] = OFF; //++임시코드
}

void SpiderPlayer::GameOver()
{
	if (curSituation == GOAL_IN) return; //이미 골인 상태라면 리턴(골인한 이후 리스폰 된 경우 예외처리)

	//UI 창 띄우기
	((GameResultUI*)(UIManager::Get()->GetUIPanel("GameResult")))
		->PrintGameOver();
	UIManager::Get()->GetUIPanel("GameResult")->SetOn(true);
}

void SpiderPlayer::GameClear()
{
	if (curSituation == GOAL_IN) return;

	//게임 클리어 처리
	isGameClear = true;
	curSituation = GOAL_IN;

	//UI 창 띄우기
	((GameResultUI*)(UIManager::Get()->GetUIPanel("GameResult")))
		->PrintGameClear();
	UIManager::Get()->GetUIPanel("GameResult")->SetOn(true);

	contextStage->SaveCurStageInfor();
}

void SpiderPlayer::Hang()
{
	if (curState == HANG) return; //++ EditTile쪽이랑 중복됨. 나중에 수정할 것

	//흔들릴 때는 cam 타겟 잠시 끄기
	Transform* t = new Transform();
	t->SetPos(pos + Vector2(200, -100));
	t->UpdateWorld();

	CAM->SetTarget(t);
	SetActionState(HANG); 
	isSpiderPhysicsOn = true; //스파이더 물리 On
	SetPendulumCondition(spiderSilk->GetFirstPlayerPos(), spiderSilk->GetAttachPos());
	SavePrePontentialEnergy();
	//CheckLeftOrRightDir(); //초기에만 설정

	SetVelocityX(0.0f); //임시코드
	SetVelocityY(0.0f);//임시코드
}

void SpiderPlayer::Respawn()
{
	Audio::Get()->Play("Damage", 0.3f);

	life--; //목숨 차감
	contextStage->UpdateLifeImage(MAX_LIFE);

	SetActionState(STAND);
	SetVelocityX(0.0f);
	SetVelocityY(0.0f);

	WebCut(); //거미줄 부착 상태였다면 자르기
	Audio::Get()->Stop("SilkCut");

	pos = playerInitialPos; //초기 위치로
	UpdateWorld();

	isRigidbodyLimitOn = true; 
	isSpiderPhysicsOn = false; //스파이더 물리 Off

	CAM->SetTarget(this);
	Drop();
	curSituation = RESPAWNED;

	if (life == 0)
	{
		GameOver();
	}
}

void SpiderPlayer::GetOffCliff()
{
	if (curState >= HANG) return; //올바르게 작동중임

	//만약 절벽에 미끄러져 떨어진 경우라면 Drop 상태로 변환
	SetActionState(DROP);
}

void SpiderPlayer::Move() 
{
	if (curState >= HANG) return;

	bool isMove = false;

	isMove |= KEY->Press('D');
	isMove |= KEY->Press('A');

	isMove ? SetActionState(MOVE) : SetActionState(STAND);
}

void SpiderPlayer::Jump()
{
	if (curState >= HANG) return; 
	if (curState == JUMP || curState == DROP) return; // 이미 공중이면 불가

	/*if (KEY->Down('W'))
	{
		Audio::Get()->Play("PlayerJump");
		SetActionState(JUMP);
	}*/

	// ---------------- [수정] ----------------
	// KEY->Down('W') 대신 아까 저장해둔 jumpBufferTime을 확인
	if (jumpBufferTime > 0.0f)
	{
		Audio::Get()->Play("PlayerJump");
		SetActionState(JUMP); // -> SpiderJump::Start()가 호출됨

		jumpBufferTime = 0.0f; // 점프했으니 버퍼 초기화 (중복 점프 방지)
	}
	// ----------------------------------------
}

void SpiderPlayer::WebShoot()
{
	if (curState == HANG) return;

	if (KEY->Down(VK_RBUTTON))
	{
		spiderSilk->SilkShoot(GetGlobalPos());
	}
}

void SpiderPlayer::WebCutCheck()
{
	if (curState != HANG) return;

	if (KEY->Down(VK_RBUTTON))
	{
		WebCut();
	}
}

void SpiderPlayer::WebCut()
{
	Audio::Get()->Play("SilkCut");

	if (curState == HANG) //매달린 상태에서 자른 거라면
	{
		curSituation = FLY_AWAY; //날아가는 상태

		// [추가된 예외처리] 
		// 땅에 붙어있거나 멈춰있는 상태(속도가 매우 느림)라면, 
		// 굳이 날아가는 물리 계산을 하지 말고 제자리에 떨어지게 함.
		if (velocity.Magnitude() < STOP_VELOCITY_THRESHOLD) // 임계값 (테스트하며 조절)
		{
			SetVelocity(Vector2(0, 0)); // 속도 0으로 초기화 (점프 방지)
			// RemainVelocityAfterCut(); // 이 함수는 호출하지 않음!
		}
		else
		{
			// 스윙 중일 때만 물리 계산 적용
			RemainVelocityAfterCut();
		}

		// [버그 해결] 상태가 변하면 RespawnTimer가 돌지 않으므로, 여기서 빨간색 효과를 꺼줘야 함.
		intValueBuffer->Get()[SWITCH_SLOT] = OFF;
		respawnedTime = 0; // 타이머도 초기화
		blinkTime = 0.0f;

		isRigidbodyLimitOn = false; //현재 받은 속도 그대로 날아가도록 임시 처리
		isSpiderPhysicsOn = false; //스파이더 물리 Off
		//mainVelocityAfterCut(); //자른 직후 현재 운동하던 방향으로 날아가도록

		CAM->SetTarget(this); //카메라 다시 타겟팅
		spiderSilk->SetParent(this);
		spiderSilk->Cut();
		Drop();
	}
	else //기타 상태에서 잘림 (리스폰 상황, 적에 거미줄이 닿음, 거미줄 부착 불가 타일에 닿음)
	{
		spiderSilk->SetParent(this);
		spiderSilk->Cut();
		spiderSilk->Update();
	}
}

void SpiderPlayer::AdjustWebLength() //줄길이 조절
{
	if (curState != HANG) return;

	// ++예외처리 보류

	//if (KEY->Press('W'))
	//{
	//	spiderSilk->AdjustSilkLength(true); //줄 조절
	//	SetPendulumCondition(spiderSilk->GetFirstPlayerPos(), spiderSilk->GetAttachPos());
	//	UpdateVelocityX();
	//	//줄 길이가 달라지면 퍼텐셜 에너지 다시 계산해야 함
	//
	//}
	//else if (KEY->Press('S'))
	//{
	//	spiderSilk->AdjustSilkLength(false); //줄 조절
	//	SetPendulumCondition(spiderSilk->GetFirstPlayerPos(), spiderSilk->GetAttachPos());
	//	UpdateVelocityX();
	//	//줄 길이가 달라지면 퍼텐셜 에너지 다시 계산해야 함
	//}
}

void SpiderPlayer::DemageEffect(float curTime)
{
	//빨간색 점멸 (0.2초 간격)
	blinkTime += DELTA;

	if (blinkTime >= BLINK_TIME)
	{
		if (intValueBuffer->Get()[SWITCH_SLOT] == OFF)
			intValueBuffer->Get()[SWITCH_SLOT] = ON;
		else
			intValueBuffer->Get()[SWITCH_SLOT] = OFF;

		blinkTime -= BLINK_TIME;
	}
}

void SpiderPlayer::SetActionState(ActionState state)
{
	if (curState == state)
		return;

	curState = state;
	spiderActions[curState]->Start();
}

void SpiderPlayer::CreateActions()
{
	spiderActions[STAND] = new Action("Textures/Spider/Action/", "Player_Stand.xml", true);
	spiderActions[MOVE] = new SpiderMove(this);
	spiderActions[JUMP] = new SpiderJump(this);
	spiderActions[HANG] = new Action("Textures/Spider/Action/", "Player_Hang.xml", true);
	spiderActions[DROP] = new SpiderDrop(this);

	//셰이더 지정
	for (pair<ActionState, Action*> action : spiderActions)
	{
		action.second->SetShader(L"Shaders/Player.hlsl");
	}

	//ValueBuffer 초기화
	intValueBuffer = new IntValueBuffer();
	intValueBuffer->Get()[SWITCH_SLOT] = OFF; //스위치
	
	colorValueBuffer = new ColorValueBuffer();
	colorValueBuffer->Get() = Float4( 0.7f, 0, 0, 0 ); //오버레이 값: 빨간색
}
