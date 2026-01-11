#pragma once

class SpiderGame : public Transform
{
private:
	UINT MAX_EDITTILE_NUM = 3;

public:
	enum DeadObjectType
	{
		OBSTACLE = 1,
		ENEMY = 2
	};

	enum TileType
	{
		//빈 공간
		CAN_EDIT = 0,
		NO_EDIT = 1,

		//일반지형
		RAGULAR = 2,

		//골인지점
		GOAL = 3,

		//장애물: 십의 자리 1 ->  DeadObjectType 1
		DOWN_THORN = 10,
		UP_THORN = 11,
		COBWEB = 12,

		//적: 천의 자리 2 ->  DeadObjectType 2
		JUMP_ENEMY = 20,
		MOVE_ENEMY = 21,
		HANG_ENEMY = 22,

		//플레이어
		PLAYER = 30
	};

public:
	SpiderGame(int loadStageNum);
	~SpiderGame();

	void Update();
	void Render();
	void PostRender();
	void Edit();

	bool GetIsEditing() { return isEditing; }
	int GetLeftEditTileNum() { return MAX_EDITTILE_NUM - editedTileNum; }

	void StartGame(); //에디팅 모드 -> 게임 모드로 변경
	void EndGame(); //게임 끝내기 -> 맵, 플레이어, 적 초기화
	void ToEditingMode(); //다시 에디팅 모드로 변경

	int GetLife() { return player->GetLife(); }
	void UpdateLifeImage(int maxLife);

	//클리어 시 클리어 정보 저장
	void SaveCurStageInfor();

private:
	//스테이지 로드 & 타일 생성
	void CreateTiles();
	void UpdateInstanceData();
	void LoadStage(int stageNum);
	Float2 ConvertTypeNumToFrame(const int& index, const Vector2& curTilePos);

	//플레이어가 맵 구역을 벗어났는지 체크 -> 벗어나면 리스폰
	void CheckPlayerInMapArea(); 

	//충돌 처리 - 에디팅 용/게임 용
	void EditingCollision(EditTileObject* editTile);
	void PlayerCollision(EditTileObject* editTile);

private:
	int curStage;
	Vector2 tileSize;
	UINT width, height; //맵 전체 크기 -> 처음 맵 로드 시 값 저장

	bool isEditing = true; //현재 에디팅 상태인지 or 게임 플레이 상태인지
	bool isInit = true; //처음 실행인지

	Quad* quad; //타일 이미지

	vector<InstanceData> instances;
	VertexBuffer* instanceBuffer;

	vector<class EditTileObject*> editTileObjects;
	Quad* curserPointTile;
	vector<int> tileTypes; 

	Vector2 selectFrame; //현재 선택된 타일
	int editedTileNum = 0; //현재 에디팅한 타일 개수: 수정은 3개까지만 가능

	//--------------------------------------
	SpiderPlayer* player; //플레이어
	Slider* lifeImage; //목숨 UI

};