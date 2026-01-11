#include "Framework.h"
#include "StartScene.h"

StartScene::StartScene()
{
	background = new Quad(L"Textures/Spider/Background/Start_Background.png");
	background->SetPos(CENTER);
	background->UpdateWorld();

	//start game 버튼
	startGameButton = new Button(L"Textures/Spider/UI/StartGame_Button.png");
	startGameButton->SetPos(CENTER_X + 400, CENTER_Y - 120);
	startGameButton->SetEvent(bind(&StartScene::OnClickStartGameButton, this));

	//Exit 버튼
	exitButton = new Button(L"Textures/Spider/UI/Exit_Button.png");
	exitButton->SetPos(CENTER_X + 400, CENTER_Y - 190);
	exitButton->SetEvent(bind(&StartScene::OnClickExitButton, this));

	//카메라 기능 끄기
	CAM->CameraOff();

	//시작화면 브금 & 버튼 효과음
	Audio::Get()->Add("BGM", "Sounds/Start_bgm.wav", true);
	Audio::Get()->Add("Button_StartScene", "Sounds/ButtonClick_Start.wav", false, false);

	//플레이어 관련 효과음
	Audio::Get()->Add("PlayerJump", "Sounds/PlayerJump.wav");
	Audio::Get()->Add("SilkCut", "Sounds/SilkCut.wav");
	Audio::Get()->Add("SilkShoot", "Sounds/SilkShoot.wav");
	Audio::Get()->Add("SilkAttach", "Sounds/SilkAttach.wav");
	Audio::Get()->Add("Damage", "Sounds/Damage.mp3");

	Audio::Get()->Play("BGM", 0.3f);
}

StartScene::~StartScene()
{
	delete background;
	delete startGameButton;
	delete exitButton;
}

void StartScene::Update()
{
	startGameButton->Update();
	exitButton->Update();
}

void StartScene::Render()
{
	background->Render();
	startGameButton->Render();
	exitButton->Render();
}

void StartScene::PostRender()
{
}

void StartScene::OnClickStartGameButton()
{
	Audio::Get()->Play("Button_StartScene");
	SceneManager::Get()->ChangeScene("Select");
}

void StartScene::OnClickExitButton()
{
	Audio::Get()->Play("Button_StartScene");
}
