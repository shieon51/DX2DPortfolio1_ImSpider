#include "Framework.h"
#include "StageScene.h"

StageScene::StageScene(int stageNum)
	: stageNum(stageNum)
{
	//게임 관리
	game = new SpiderGame(stageNum);

	//배경화면
	wstring fileStr = L"Textures/Spider/Background/Stage" 
		+ to_wstring(stageNum) + L"_Background.png";
	background = new Quad(fileStr);
	background->SetPos(CENTER);
	background->UpdateWorld();

	//게임시작 버튼
	gameStartButton = new Button(L"Textures/Spider/UI/Button.png");
	gameStartButton->SetPos(SCREEN_WIDTH - 100, 50);
	gameStartButton->SetScale(0.5f, 0.5f);
	gameStartButton->SetEvent(bind(&StageScene::OnClickStartButton, this));
	gameStartButton->SetText("GAME START");

	//게임 나가기 버튼
	gameExitButton = new Button(L"Textures/Spider/Icon/exitLeft.png");
	gameExitButton->SetPos(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 50);
	gameExitButton->SetEvent(bind(&StageScene::OnClickExitButton, this));
}

StageScene::~StageScene()
{
	delete game;
	delete gameStartButton;
	delete gameExitButton;
	delete background;
}

void StageScene::Update()
{
	background->SetPos(CAM->GetPos() + CENTER);
	background->UpdateWorld();

	if (game->GetIsEditing())
		gameStartButton->Update();
	else
		gameExitButton->Update();

	((GameResultUI*)(UIManager::Get()->GetUIPanel("GameResult")))->Update(); //?

	game->Update();
}

void StageScene::Render()
{
	background->Render();

	game->Render();
}

void StageScene::PostRender()
{
	game->Edit();

	if (game->GetIsEditing())
		gameStartButton->Render();
	else
		gameExitButton->Render();

	game->PostRender();

	//게임 결과 출력창
	if (UIManager::Get()->GetUIPanel("GameResult")->IsOn())
		UIManager::Get()->GetUIPanel("GameResult")->PanelVisiable();
}

void StageScene::Start()
{
	game->ToEditingMode();
}

void StageScene::End()
{
	game->EndGame();
}

void StageScene::OnClickStartButton()
{
	string tagStr = "Stage" + to_string(stageNum);

	Audio::Get()->Play(tagStr, 0.2f);
	game->StartGame();
}

void StageScene::OnClickExitButton() //게임 도중 나가기 버튼
{
	string stageNumStr = to_string(stageNum);

	SceneManager::Get()->ChangeScene("Select");
	Audio::Get()->Stop("Stage" + stageNumStr);

	Audio::Get()->Play("BGM", 0.3f);
}
