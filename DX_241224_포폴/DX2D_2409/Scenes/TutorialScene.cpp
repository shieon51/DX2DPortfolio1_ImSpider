#include "Framework.h"
#include "Scenes/TutorialScene.h"

TutorialScene::TutorialScene()
{
	//게임 관리
	game = new SpiderGame(0);

	//배경화면
	background = new Quad(L"Textures/Spider/Background/Stage0_Background.png");
	background->SetPos(CENTER);
	background->UpdateWorld();

	//튜토리얼 이미지 불러오기
	tutorialImages.resize(8);
	int i = 0;
	for (Quad*& image : tutorialImages)
	{
		wstring str = L"Textures/Spider/Tutorial/Tutorial_image" + to_wstring(i) + L".png";
		image = new Quad(str);
		image->SetTag(to_string(i));
		image->SetPos(IMAGE_POS[i++]); 
		image->UpdateWorld();
	}

	//게임시작 버튼
	gameStartButton = new Button(L"Textures/Spider/UI/Button.png");
	gameStartButton->SetPos(SCREEN_WIDTH - 100, 50);
	gameStartButton->SetScale(0.5f, 0.5f);
	gameStartButton->SetEvent(bind(&TutorialScene::OnClickStartButton, this));
	gameStartButton->SetText("GAME START");

	//게임 나가기 버튼
	gameExitButton = new Button(L"Textures/Spider/Icon/exitLeft.png");
	gameExitButton->SetPos(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50);
	gameExitButton->SetEvent(bind(&TutorialScene::OnClickExitButton, this));

	//튜토리얼 전용 UI
	tutorialUI = (TutorialUI*)(UIManager::Get()->GetUIPanel("Tutorial"));
}

TutorialScene::~TutorialScene()
{
	delete game;
	delete gameStartButton;
	delete gameExitButton;
	delete background;

	for (Quad* image : tutorialImages)
		delete image;
}

void TutorialScene::Update()
{
	background->SetPos(CAM->GetPos() + CENTER);
	background->UpdateWorld();

	if (game->GetIsEditing())
	{
		switch (tutorialUI->GetCurPage())
		{
		case 0:
			//카메라가 오른쪽으로 일정 수치 이상 이동하면
			if (CAM->GetPos().x > 150.0f) 
				tutorialUI->NextPage(); //다음 페이지로
			break;
		case 1:
			//모든 타일을 다 놓았다면
			if (game->GetLeftEditTileNum() == 0)
				tutorialUI->NextPage(); //다음 페이지로
			break;
		case 2:
			break;
		}
		gameStartButton->Update();
	}
	else
	{
		gameExitButton->Update();
	}

	//if (UIManager::Get()->GetUIPanel("GameResult")->IsOn())
	((GameResultUI*)(UIManager::Get()->GetUIPanel("GameResult")))->Update(); //?

	game->Update();
}
	
void TutorialScene::Render()
{
	background->Render();

	if (!game->GetIsEditing()) //게임 중인 경우에만 이미지 출력
	{
		for (Quad* image : tutorialImages)
			image->Render();
	}

	game->Render();
}

void TutorialScene::PostRender()
{
	game->Edit();

	//--
	//for (Quad* image : tutorialImages)
	//	image->Edit();

	if (game->GetIsEditing()) //아직 에디팅 중이면
	{
		tutorialUI->PanelVisiable();
		gameStartButton->Render();
	}
	else
		gameExitButton->Render();

	game->PostRender();

	//게임 결과 출력창, 게임 그만하기 버튼 등
	if (UIManager::Get()->GetUIPanel("GameResult")->IsOn())
		UIManager::Get()->GetUIPanel("GameResult")->PanelVisiable();
}

void TutorialScene::Start()
{
	game->ToEditingMode();
}

void TutorialScene::End()
{
	game->EndGame();
}

void TutorialScene::OnClickStartButton()
{
	Audio::Get()->Play("Stage0", 0.5f);
	game->StartGame();
	tutorialUI->SetOn(false);
}

void TutorialScene::OnClickExitButton()
{
	UIManager::Get()->GetUIPanel("GameResult")->SetOn(false);
	SceneManager::Get()->ChangeScene("Select");
	Audio::Get()->Stop("Stage0");

	Audio::Get()->Play("BGM", 0.3f);
}
