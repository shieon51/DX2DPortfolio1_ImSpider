#include "Framework.h"

GameResultUI::GameResultUI()
{
	FONT->AddColor("GameResult", 0.4f, 0.1f, 0.8f);
	FONT->AddStyle("GameResult", L"읏맨체", 40);
	
	SetPos(CENTER);
	UpdateSize(Vector2(600, 400));
	UpdateWorld();

	// 게임 결과(win/lose) 이미지
	imageWin = new Quad(L"Textures/Spider/UI/Win_image.png");
	imageLose = new Quad(L"Textures/Spider/UI/Lose_image.png");
	imageWin->SetPos(CENTER);
	imageLose->SetPos(CENTER);
	imageWin->UpdateWorld();
	imageLose->UpdateWorld();

	//돌아가기 버튼
	buttons.push_back(new Button(L"Textures/Spider/UI/Button.png"));
	buttons.back()->SetPos(CENTER_X, CENTER_Y - 110);
	buttons.back()->SetScale(0.5f, 0.5f);
	buttons.back()->SetEvent(bind(&GameResultUI::OnClickButton, this));
	buttons.back()->SetText("돌아가기");
	buttons.back()->UpdateWorld();

}

GameResultUI::~GameResultUI()
{
}

void GameResultUI::Update()
{
	if (!isOn) return;

	// 본체(Quad) 행렬 업데이트
	UpdateWorld();

	// 내부 이미지 행렬 업데이트 (카메라 영향 등을 받을 수 있으므로 안전하게)
	if (isGameClear) imageWin->UpdateWorld();
	else imageLose->UpdateWorld();

	for (Button* button : buttons)
		button->Update();

}

void GameResultUI::PanelVisiable()
{
	if (!isOn) return;

	FONT->SetColor("GameResult");
	FONT->SetStyle("GameResult");

	Vector2 offset = CENTER + Vector2(-30, 110);
	string resultStr[] =
	{
		"GAME OVER", "GAME CLEAR"
	};

	Quad::Render();
	FONT->RenderText(resultStr[isGameClear], offset);

	// 승패 여부에 따라 다른 이미지 출력
	if (isGameClear)
	{
		imageWin->Render();
	}
	else
	{
		imageLose->Render();
	}

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
