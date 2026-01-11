#pragma once

class StageSelectScene : public Scene
{
private:
	enum StageName
	{
		TUTORIAL = 0,
		EASY = 1,
		NORMAL = 2,
		HARD = 3
	};

public:
	StageSelectScene();
	~StageSelectScene();

	void Update() override;
	void Render() override;
	void PostRender() override;

	void Start() override;

	void OnClickStageButton(int stageNum);
	//void OnEnteredStageButton();
private:
	void LoadCurStageInfor(); //다음 클리어 차례인 스테이지 정보 불러오기

private:
	//배경화면
	Quad* background;

	//스테이지 버튼
	Button* stageButtons[4];

	//버튼 잠김 이미지
	Quad* lockButtonImages[4];

	//현재 클리어 차례인 스테이지를 표시하는 이미지(거미 이미지)
	Quad* curStage;
	int curStageNum = 0; 

	//ValueBuffer
	IntValueBuffer* intValueBuffer;
	ColorValueBuffer* colorValueBuffer;
};