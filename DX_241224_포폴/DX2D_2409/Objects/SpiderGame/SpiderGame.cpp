#include "Framework.h"

SpiderGame::SpiderGame(int loadStageNum)
	: curStage(loadStageNum)
{
	SetTag(to_string(curStage));

	pos = { 50, 120 };
	UpdateWorld();

	player = new SpiderPlayer();
	player->SetContextStage(this);
	player->SetActive(false);

	//에디팅 시 현재 마우스가 가리키고 있는 벽돌 표시(반투명 벽돌)
	curserPointTile = new Quad(L"Textures/Spider/Tile/tile2.png");
	curserPointTile->GetMaterial()->SetColor(1, 1, 1, 0.4f);
	curserPointTile->SetPos(-1, -1);

	//목숨 표시
	lifeImage = new Slider(L"Textures/Spider/UI/Life.png", L"Textures/Spider/UI/Life_Back.png");
	lifeImage->SetPos(100, SCREEN_HEIGHT - 50);
	lifeImage->UpdateWorld();
}

SpiderGame::~SpiderGame()
{
	delete quad;
	delete instanceBuffer;
	delete player;
	delete lifeImage;
	delete curserPointTile;

	for (EditTileObject* editTile : editTileObjects)
		delete editTile;

	for (InstanceData instance : instances)
		delete &instance;

	EnemyManager::Delete();
}

void SpiderGame::Update()
{
	if (isEditing) // 에디팅 중일 때
	{
		for (EditTileObject* editTile : editTileObjects)
		{
			EditingCollision(editTile);
		}
	}
	else //게임 중일 때
	{
		CheckPlayerInMapArea();
		player->Update();

		//플레이어 & 적 충돌
		EnemyManager::Get()->Update();

		for (EditTileObject* editTile : editTileObjects)
		{
			//플레이어 & 타일 충돌
			PlayerCollision(editTile);

			//플레이어 줄 & 타일 충돌
			editTile->Update();

			//적 & 플레이어 줄/타일 충돌
			EnemyManager::Get()->CollisionMapAndSilkCheck(editTile);
		}
	}
}

void SpiderGame::Render()
{
	instanceBuffer->Set(1);

	quad->GetMaterial()->Set();
	quad->GetMesh()->SetMesh();

	deviceContext->DrawIndexedInstanced(6, editTileObjects.size(), 0, 0, 0);

	for (EditTileObject* editTile : editTileObjects)
		editTile->Render();

	if (!isEditing) //게임 중일때만 플레이어 render
	{
		EnemyManager::Get()->Render();
		player->Render();
	}
	else if(curserPointTile->GetPos() != Vector2(-1, -1))
	{
		curserPointTile->Render();
	}
}

void SpiderGame::PostRender()
{
	if (isEditing)
	{
		Vector2 offset = { SCREEN_WIDTH - 100, 110 };
		string remainTileNumStr = "남은 타일 수: " + to_string(GetLeftEditTileNum());

		FONT->RenderText(remainTileNumStr, offset);
	}
	else
	{
		lifeImage->Render();
	}
}

void SpiderGame::Edit()
{
	if (isEditing) //에디팅 중일때만 버튼 보이기
	{
		//++ 나중에 폰트 이용해서 글자 출력하기
		ImGui::Text("Number of tiles remaining: %d", MAX_EDITTILE_NUM - editedTileNum);
		//RenderSampleButtons();
	}

	player->Edit();
}

void SpiderGame::StartGame()
{
	//빈 공간 콜라이더는 끄기 (타입 1)
	//isEditing false로 변경
	//타입 0 중 에디팅 된 애만 콜라이더 남기기 (GetIsEdited() 이용)
	//적, 플레이어 위치 타일도 콜라이더 끄기
	//EnemyManager로 적 미리 생성, 플레이어 미리 생성한 후 active만 키기

	for (EditTileObject* editTile : editTileObjects)
	{
		if (editTile->GetTileType() == NO_EDIT) //에디팅 불가 지역
		{
			editTile->GetInstanceData()->curFrame = { 0, 0 }; //빈 타일로 바꿔준 후
			UpdateInstanceData();
			editTile->SetActive(false);
		}
		else if (editTile->GetTileType() == CAN_EDIT && !editTile->GetIsEdited()) //에디팅 가능 지역이나 에디팅 안한 경우
		{
			editTile->SetActive(false);
		}
		else if (editTile->GetTileType() == CAN_EDIT && editTile->GetIsEdited()) //에디팅 가능 지역 & 에디팅 됨
		{
			editTile->SetTileType(RAGULAR); //에디팅 된 타일 ->일반타일 뿐임
		}
		else if (editTile->GetTileType() == PLAYER) //플레이어 타일
		{
			editTile->GetInstanceData()->curFrame = { 0, 0 }; 
			UpdateInstanceData();
			editTile->SetActive(false);
			player->SetActive(true); //플레이어 켜주기
		}
		else if (editTile->GetTileType() / 1000 == DeadObjectType::ENEMY) //적 타일
		{
			editTile->GetInstanceData()->curFrame = { 0, 0 };
			UpdateInstanceData();
			editTile->SetActive(false);
		}
		else if (editTile->GetTileType() == UP_THORN)
		{
			Vector2 tileSize = editTile->Size() * Vector2(1, 0.5f);
			Vector2 offset = { 0, -tileSize.x * 0.25f}; //콜라이더 크기 변경해주기
			editTile->Translate(offset);
			editTile->UpdateWorld();
			editTile->UpdateSize(tileSize);
		}
		else if (editTile->GetTileType() == DOWN_THORN)
		{
			Vector2 tileSize = editTile->Size() * Vector2(1, 0.5f);
			Vector2 offset = { 0, +tileSize.x * 0.25f }; //콜라이더 크기 변경해주기
			editTile->Translate(offset);
			editTile->UpdateWorld();
			editTile->UpdateSize(tileSize);
		}
	}

	isEditing = false;

	//적 객체들 활성화
	EnemyManager::Get()->TurnOnEnemyActivity();

	CAM->SetTarget(player);
	player->Drop();
}

void SpiderGame::EndGame()
{
	//적 비활성화
	EnemyManager::Get()->TurnOffEnemyActivity();

	//플레이어 비활성화, 목숨, 골인여부체크, 설치 타일 가능 개수 원래대로
	player->SetActive(false);
	player->PlayerReset();

	// [버그 수정] EndGame 시점에 들리는 모든 거미줄 관련 소리 강제 정지
	// (PlayerReset 직후에 꺼야 '틱' 하는 소리조차 안 들리게 할 수 있음)
	Audio::Get()->Stop("SilkCut");    // 방금 WebCut 때문에 난 소리 끄기
	Audio::Get()->Stop("SilkShoot");  // 쏘는 중이었다면 끄기
	Audio::Get()->Stop("SilkAttach"); // 붙는 소리 끄기

	UpdateLifeImage(player->GetMaxLife());
	editedTileNum = 0;

	//에디팅 모드로 전환
	isEditing = true;

	//카메라 기능 끄기
	CAM->SetTarget(nullptr);
	CAM->CameraOff();

	//적 삭제
	EnemyManager::Get()->ClearEnemys();

	//(두번째 실행부터는 파일 로드 필요 없음)
	isInit = false;
}

void SpiderGame::ToEditingMode()
{
	EnemyManager::Get()->SetSpiderPlayer(player);

	if (isInit) //초기 실행이면
	{
		LoadStage(curStage);
		CreateTiles();
	}
	else // 재실행이면 -> 로드하지 않고 타일 이미지만 변경
	{
		//타일들 에디팅 모드로
		int y = 0, x = 0;
		for (EditTileObject* editTile : editTileObjects)
		{
			int index = y * width + (x++);

			//프레임 에디팅 용으로 바꿔주기 + 플레이어 초기 위치로
			editTile->GetInstanceData()->curFrame = ConvertTypeNumToFrame(index, editTile->GetGlobalPos());
			UpdateInstanceData();

			//에디팅으로 추가된 타일인 경우 -> '수정되지 않음'으로 변경
			if (editTile->GetIsEdited())
			{
				editTile->SetTileType(CAN_EDIT);
				editTile->SetIsEdited(false);
			}

			//collider 사이즈와 위치가 일부 변경된 타일인 경우 -> 원래대로
			if (editTile->GetTileType() == UP_THORN)
			{
				//위치 원래대로
				Vector2 offset = { 0, +tileSize.x * 0.25f };
				editTile->Translate(offset);
				editTile->UpdateWorld();
				//타일 사이즈 원래대로
				editTile->UpdateSize(tileSize);
			}
			else if (editTile->GetTileType() == DOWN_THORN)
			{
				//위치 원래대로
				Vector2 offset = { 0, -tileSize.x * 0.25f };
				editTile->Translate(offset);
				editTile->UpdateWorld();
				//타일 사이즈 원래대로
				editTile->UpdateSize(tileSize);
			}

			//모든 타일 켜기
			editTile->SetActive(true);

			if (x == width)
			{
				y++;
				x = 0;
			}
		}
	}
}

void SpiderGame::UpdateLifeImage(int maxLife)
{
	lifeImage->SetAmount((float)player->GetLife() / maxLife);
	lifeImage->UpdateWorld();
}

void SpiderGame::CheckPlayerInMapArea()
{
	//구역을 벗어나면 -> 리스폰하기
	if (player->GetGlobalPos().x < -tileSize.x * 2
		|| player->GetGlobalPos().x > tileSize.x * (width + 2)
		|| player->GetGlobalPos().y < -tileSize.y
		|| player->GetGlobalPos().y > tileSize.x * (height + 1))
	{
		player->Respawn();
	}
}

void SpiderGame::CreateTiles()
{
	Vector2 maxFrame = { 7, 2 };

	quad = new Quad(L"Textures/Spider/Tiles.png", Vector2(),
		Vector2(1 / maxFrame));
	quad->GetMaterial()->SetShader(L"Shaders/Instancing.hlsl");

	Vector2 tileSize = quad->GetSize();

	//나중에 엑셀 로드 과정에서 조절되도록 (최대 가로 26, 세로 10)
	UINT size = width * height;

	instances.resize(size);
	editTileObjects.reserve(size);

	//타일 사이즈 저장 (맵 전체 구역 파악을 위함)
	this->tileSize = tileSize;
	EnemyManager::Get()->SetTileSize(tileSize);


	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y * width + x;
			EditTileObject* editTile;

			//* 타입에 따라 나누기 - 일반, 장애물, 기타(에디팅용)
			if (tileTypes[index] / 10 == DeadObjectType::OBSTACLE)	// 장애물
				editTile = new ObstacleTile(tileSize, &instances[index]); 
			else if (tileTypes[index] == TileType::RAGULAR)			// 일반 지형
				editTile = new BasicTile(tileSize, &instances[index]);
			else if (tileTypes[index] == TileType::GOAL)			// 골인 
				editTile = new GoalTile(tileSize, &instances[index]);
			else													// 그 외 (적 타일, 플레이어 타일, 빈공간(수정가능, 불가능))
				editTile = new BasicTile(tileSize, &instances[index]);
			
			Vector2 curTilePos = tileSize * Vector2(x, -y + height - 1) - Vector2(10, 10);

			editTile->SetPos(curTilePos);
			editTile->SetParent(this);
			editTile->UpdateWorld();
			editTile->SetPlayer(player);
			editTile->SetSilkCollider(player->GetSpiderSilk()->GetSilkCollider());
			editTile->SetTileType(tileTypes[index]); // 타일 타입 넘겨주기 -> 타입에 따라 에디팅 가능/불가능 하도록
			instances[index].transform = XMMatrixTranspose(editTile->GetWorld());
			instances[index].curFrame = ConvertTypeNumToFrame(index, editTile->GetGlobalPos()); // 타입을 이미지 프레임으로 변환
			instances[index].maxFrame = maxFrame;
			editTileObjects.push_back(editTile);
		}
	}

	instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), size);

	//현재 선택된 타일 초기화
	selectFrame = { 2, 0 };

	//카메라 움직임 범위 제한
	CAM->SetCameraLimitRange(
		-tileSize.x, tileSize.x * (width + 1),
		-tileSize.y * 2, tileSize.y * (height + 1));
}

void SpiderGame::UpdateInstanceData()
{
	instanceBuffer->Update(instances.data(), instances.size());
}

void SpiderGame::LoadStage(int stageNum)
{
	string fileName = "DataTables/SpiderGame/Stage" + to_string(stageNum) + ".csv";

	ifstream loadFile(fileName);

	string line;

	int x = 0, y = 0;

	while (getline(loadFile, line))
	{
		stringstream ss(line);
		string item;

		while (getline(ss, item, ','))
		{
			tileTypes.push_back(stoi(item)); //정보 불러오기

			if (y >= 1) continue;
			x++;
		}

		y++;
	}

	//현재 맵 크기 정보 저장
	width = x;
	height = y;
}

Float2 SpiderGame::ConvertTypeNumToFrame(const int& index, const Vector2& curTilePos)
{
	Float2 frame;

	int tileType = tileTypes[index];

	if (tileType <= GOAL) //빈 공간(수정가능/불가능), 일반지형, 골인
	{
		frame = { (float)tileType, 0 };
	}
	else if (tileType / 10 == DeadObjectType::OBSTACLE) //장애물
	{
		frame = { (float)tileType - 6, 0 };
	}
	else if (tileType / 1000 == DeadObjectType::ENEMY) //적: 앞 두자리는 적종류, 뒤 두자리는 속성값
	{
		frame = { (float)(tileType / 100 % 10), 1 };

		EnemyManager::Get()->AddSpiderEnemy(curTilePos, tileType); //적 만들고 푸시
	}
	else if (tileType == PLAYER) //플레이어
	{
		frame = { (float)(tileType / 10), 1 };
		player->SetPlayerInitialPos(curTilePos);
		player->SetPos(curTilePos); //초기 위치 설정
		player->SetLastLandCollisionTilePos(curTilePos);
	}
	else //예외
		frame = { 0, 0 };

	return frame;
}

void SpiderGame::EditingCollision(EditTileObject* editTile)
{
	if (editTile->IsPointCollision(CAM->ScreenToWorld(mousePos))) //현재 카메라뷰 포지션에 맞게
	{
		if (editTile->GetTileType() == CAN_EDIT)
		{
			if (KEY->Up(VK_LBUTTON))
			{
				// (UI가 앞에 떠 있는 곳 클릭하면 -> 뒤에 클릭 반영 안 되도록 예외처리)
				if (UIManager::Get()->IsContain("Tutorial", mousePos)) return;

				if (editedTileNum < MAX_EDITTILE_NUM	 //설치 가능 타일수 제한
					&& editTile->GetIsEdited() == false) //아직 수정되지 않은 타일이면
				{
					editTile->GetInstanceData()->curFrame = selectFrame;
					editTile->SetIsEdited(true);
					editedTileNum++;
				}
				else if (editTile->GetIsEdited() == true) //이미 수정된 적 있는 타일이면 취소하기
				{
					editTile->GetInstanceData()->curFrame = { 0, 0 };
					editTile->SetIsEdited(false);
					editedTileNum--;
				}
				UpdateInstanceData();
			}
			else  //마우스만 올려놓은 상태라면 -> 현재 가리키고 있는 타일 표시하기
			{
				curserPointTile->SetPos(editTile->GetGlobalPos());
				curserPointTile->UpdateWorld();
			}
		}
		else //에디팅 가능 지역이 아니라면
		{
			curserPointTile->SetPos(-1, -1);
			curserPointTile->UpdateWorld();
		}
	}
}

void SpiderGame::PlayerCollision(EditTileObject* editTile)
{
	//editTile 타입에 따라 충돌 체크하기
	switch (editTile->GetTileType()) 
	{
	case RAGULAR:
		((BasicTile*)editTile)->CollisionCheck();
		break;
	case DOWN_THORN: case UP_THORN: case COBWEB:
		((ObstacleTile*)editTile)->CollisionCheck();
		break;
	case GOAL:
		((GoalTile*)editTile)->CollisionCheck();
		break;
	}
}

void SpiderGame::SaveCurStageInfor()
{
	int savedMaxStage = 0;

	//이전 스테이지 클리어 정보 불러오기
	FILE* fileCheck = nullptr;
	if (fopen_s(&fileCheck, "TextData/SpiderGame/CurStage.srt", "rb") == 0)
	{
		fclose(fileCheck); // 확인했으니 닫고

		// 원래 코드: 읽기
		BinaryReader* reader = new BinaryReader("TextData/SpiderGame/CurStage.srt");
		savedMaxStage = reader->Int();
		delete reader;
	}

	// "현재 깬 스테이지의 다음 단계"가 "저장된 기록"보다 클 때만 저장
	// (예: 기록은 4인데 1을 깼다면, 2는 4보다 작으므로 저장 안 함 -> 기록 유지)
	int nextOpenStage = curStage + 1;

	// 만약 현재 플레이한 스테이지가 최초 클리어인 경우
	if (nextOpenStage > savedMaxStage)
	{
		BinaryWriter* writer = new BinaryWriter("TextData/SpiderGame/CurStage.srt");
		writer->Int(curStage + 1);
		delete writer;
	}
}