#pragma once

class StageScene : public Scene
{
public:
	StageScene(int stageNum);
	~StageScene();

	void Update() override;
	void Render() override;
	void PostRender() override;

	void Start() override;
	void End() override;

	void OnClickStartButton();
	void OnClickExitButton();

private:
	SpiderGame* game;

	int stageNum;

	//시작 버튼 & 나가기 버튼
	Button* gameStartButton;
	Button* gameExitButton;

	Quad* background;
};