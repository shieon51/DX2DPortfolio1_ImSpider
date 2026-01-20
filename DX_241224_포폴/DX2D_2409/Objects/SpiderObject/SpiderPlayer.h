#pragma once

class SpiderPlayer : public SpiderPhysics 
{
private:
	const int MAX_LIFE = 3;
	const float MAX_RESPAWNED_TIME = 3.0f; //
	const float BLINK_TIME = 0.2f;
	const int SWITCH_SLOT = 0;
public:
	enum DemegeSwitch
	{
		OFF, ON
	};

	enum ActionState
	{
		STAND, MOVE, HANG, JUMP, DROP
	};

	enum GameProgressState //기본, 끊고 날아갈 때, 리스폰된 상황, 골인, 게임오버
	{
		DEFAULT, FLY_AWAY, RESPAWNED, GOAL_IN, GAME_OVER
	};

public:
	SpiderPlayer();
	~SpiderPlayer();

	void Update();
	void Render();
	void Edit();

	void Land();
	void Drop();
	void Floating(); // jump, drop 상태에서만 사용
	void RespawnTimer();

	//SpiderGame에서 호출 (목숨 UI, 게임 리셋 등)
	void SetContextStage(class SpiderGame* stage) { contextStage = stage; }
	int GetMaxLife() { return MAX_LIFE; }
	int GetLife() { return life; }
	void PlayerReset();

	void GameOver();
	void GameClear();

	//EditTile에서 호출
	void Hang();
	bool IsHanging() { return curState == HANG; } 

	//ObstacleTile에서 호출 //EnemyManager, 장애물타일 에서 호출
	void Respawn();
	void WebCut(); // 거미줄 자르기

	//BasicTile에서 호출
	void GetOffCliff(); //점프하지 않고 낮은 곳으로 이동할 경우 예외처리

	//Move Action에서 호출
	GameProgressState GetCurSituation() { return curSituation; }

	PlayerSpiderSilk* GetSpiderSilk() { return spiderSilk; }
	
	Vector2 GetLastLandCollisionTilePos() { return lastLandCollisionTilePos; }
	void SetLastLandCollisionTilePos(Vector2 pos) { lastLandCollisionTilePos = pos; }
	void SetPlayerInitialPos(Vector2 pos) { playerInitialPos = pos; }

private:
	void Move();
	void Jump();
	void WebShoot(); //거미줄 발사
	void WebCutCheck(); //거미줄 자르는 버튼을 눌렀는지 체크
	void DemageEffect(float curTime);

	void SetActionState(ActionState state);

	void CreateActions();

private:
	class SpiderGame* contextStage;

	int life; //목숨
	bool isGameClear = false;
	float respawnedTime = 0;
	float blinkTime = 0;

	// 점프 키 입력 기억 시간 - 버퍼 이용해 점프 씹힘 현상 수정
	float jumpBufferTime = 0.0f;
	const float MAX_COYOTE_TIME = 0.2f;
	const float STOP_VELOCITY_THRESHOLD = 10.0f;

	ActionState curState;
	GameProgressState curSituation;
	map<ActionState, Action*> spiderActions;

	PlayerSpiderSilk* spiderSilk;

	Vector2 lastLandCollisionTilePos; //절벽지점 (중력작용) 예외처리
	Vector2 playerInitialPos; //플레이어 시작 위치 (리스폰 목적)

	//셰이더 ValueBuffer (충돌 시 일정시간 적용)
	IntValueBuffer* intValueBuffer;
	ColorValueBuffer* colorValueBuffer;
};