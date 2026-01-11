#include "Framework.h"

GameResultUI::GameResultUI()
{
	FONT->AddColor("GameResult", 0.4f, 0.1f, 0.8f);
	FONT->AddStyle("GameResult", L"읏맨체", 40);
	
	SetPos(CENTER);
	UpdateSize(Vector2(600, 400));
	UpdateWorld();

	//돌아가기 버튼
	buttons.push_back(new Button(L"Textures/Spider/UI/Button.png"));
	buttons.back()->SetPos(CENTER_X, CENTER_Y - 100);
	buttons.back()->SetScale(0.5f, 0.5f);
	buttons.back()->SetEvent(bind(&GameResultUI::OnClickButton, this));
	buttons.back()->SetText("돌아가기");
	buttons.back()->UpdateWorld();

	//++ 다음 스테이지로 버튼

}

GameResultUI::~GameResultUI()
{
}

void GameResultUI::Update()
{
	if (!isOn) return;

	for (Button* button : buttons)
		button->Update();
}

void GameResultUI::PanelVisiable()
{
	if (!isOn) return;

	FONT->SetColor("GameResult");
	FONT->SetStyle("GameResult");

	Vector2 offset = CENTER + Vector2(-30, 100);
	string resultStr[] =
	{
		"GAME OVER", "GAME CLEAR"
	};

	Quad::Render();
	FONT->RenderText(resultStr[isGameClear], offset);

	for (Button* button : buttons)
		button->Render();
}

void GameResultUI::PrintGameClear()
{
	isGameClear = true;
}

void GameResultUI::PrintGameOver()
{
	isGameClear = false;
}

void GameResultUI::OnClickButton()
{
	//스테이지 선택 화면으로 돌아가기 버튼
	UIManager::Get()->GetUIPanel("GameResult")->SetOn(false);
	SceneManager::Get()->ChangeScene("Select");
	Audio::Get()->Stop("Stage0");
	Audio::Get()->Stop("Stage1");
	Audio::Get()->Stop("Stage2");
	Audio::Get()->Stop("Stage3");

	Audio::Get()->Play("BGM", 0.3f);
}
