#include "Framework.h"

TutorialUI::TutorialUI()
{
	FONT->AddColor("Tutorial", 0.4f, 0.1f, 0.8f);
	FONT->AddStyle("Tutorial", L"국민연금체", 25);

	SetPos(CENTER_X, 75.0f);
	UpdateSize(Vector2(SCREEN_WIDTH, 150.0f));
	UpdateWorld();

	//튜토리얼 관련 이미지들 로드
	for (int i = 0; i < 3; i++)
	{
		Quad* image = new Quad(L"Textures/Spider/Tutorial/Tutorial_infor" + to_wstring(i) + L".png");
		tutorialInfors.push_back(image);
		tutorialInfors.back()->SetPos(250, 75);
		tutorialInfors.back()->UpdateWorld();
	}
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::PanelVisiable()
{
	if (!isOn) return;

	FONT->SetColor("Tutorial");
	FONT->SetStyle("Tutorial");

	Vector2 offset(480, 75);
	string inforStr[] =
	{
		"카메라를 이동해보세요. (WASD + 마우스 우클릭)",
		"빈 공간에 타일을 놓아보세요.",
		"준비되었나요? 게임을 시작해보아요."
	};

	Quad::Render();
	tutorialInfors[curPage]->Render();

	FONT->RenderText(inforStr[curPage], offset);
}
