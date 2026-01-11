#pragma once

class TutorialScene : public Scene
{
private:
	//튜토리얼 이미지 각 위치
	const Vector2 IMAGE_POS[8] =
	{
		Vector2(103, 300), Vector2(242, 310),
		Vector2(452, 310), Vector2(725, 274),
		Vector2(1123, 451), Vector2(559, 555),
		Vector2(1619, 365), Vector2(2216, 324)
	};

public:
	TutorialScene();
	~TutorialScene();

	void Update() override;
	void Render() override;
	void PostRender() override;

	void Start() override;
	void End() override;

	void OnClickStartButton();
	void OnClickExitButton();

private:
	SpiderGame* game;
	
	//튜토리얼 UI
	TutorialUI* tutorialUI;
	vector<Quad*> tutorialImages;

	//시작 버튼
	Button* gameStartButton;
	
	//나가기 버튼
	Button* gameExitButton;

	Quad* background;
};