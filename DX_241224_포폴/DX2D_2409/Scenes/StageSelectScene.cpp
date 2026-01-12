#include "Framework.h"
#include "StageSelectScene.h"

StageSelectScene::StageSelectScene()
{
	//배경화면
	background = new Quad(L"Textures/Spider/Background/StageSelect_Background.png");
	background->SetPos(CENTER);
	background->UpdateWorld();

	//스테이지 버튼
	wstring fileStr[4] =
	{
		L"Textures/Spider/UI/Tutorial_Button.png",
		L"Textures/Spider/UI/Easy_Button.png",
		L"Textures/Spider/UI/Normal_Button.png",
		L"Textures/Spider/UI/Hard_Button.png",
	};

	for (int i = 0; i < 4; i++)
	{
		stageButtons[i] = new Button(fileStr[i]);
		stageButtons[i]->SetPos(CENTER_X - 450 + 300 * i, CENTER_Y + 100);
		stageButtons[i]->SetIntParamEvent(bind(&StageSelectScene::OnClickStageButton, this, placeholders::_1));
		stageButtons[i]->SetIntParam(i);
		stageButtons[i]->SetActive(false);

		lockButtonImages[i] = new Quad(fileStr[i]);
		lockButtonImages[i]->SetPos(stageButtons[i]->GetPos());
		lockButtonImages[i]->GetMaterial()->SetShader(L"Shaders/Player.hlsl");
		lockButtonImages[i]->UpdateWorld();
	}

	intValueBuffer = new IntValueBuffer();
	intValueBuffer->Get()[0] = 1; //++ 임시 코드
	colorValueBuffer = new ColorValueBuffer();
	colorValueBuffer->Get() = Float4(0, 0, 0, 0);

	//현재 진행해야 하는 스테이지 표시(거미 이미지)
	curStage = new Quad(L"Textures/Spider/UI/Select.png");

	//UI 미리 추가 (튜토리얼, 기타 상태창)
	UIManager::Get()->AddUIPanel("Tutorial", new TutorialUI());
	UIManager::Get()->AddUIPanel("GameResult", new GameResultUI());
	//++

	//버튼 효과음 및 스테이지별 배경음악
	//Audio::Get()->Add("ButtonEnter_Select", "Sounds/ButtonEnter_Select.wav");
	Audio::Get()->Add("ButtonClick_Select", "Sounds/ButtonClick_Select.wav");
	Audio::Get()->Add("Stage0", "Sounds/Stage0_bgm.wav", false, true);
	Audio::Get()->Add("Stage1", "Sounds/Stage1_bgm.mp3", false, true);
	Audio::Get()->Add("Stage2", "Sounds/Stage2_bgm.wav", false, true);
	Audio::Get()->Add("Stage3", "Sounds/Stage3_bgm.wav", false, true);
}

StageSelectScene::~StageSelectScene()
{
	delete background;
	delete curStage;
	delete intValueBuffer;
	delete colorValueBuffer;

	for (Button* button : stageButtons)
		delete button;

	for (Quad* image : lockButtonImages)
		delete image;
}

void StageSelectScene::Update()
{
	curStage->UpdateWorld();

	for (Button* button : stageButtons)
		button->Update();
}

void StageSelectScene::Render()
{
	intValueBuffer->SetPS(2);
	colorValueBuffer->SetPS(3);

	background->Render();
	curStage->Render();

	for (Quad* image : lockButtonImages)
		image->Render();

	for (Button* button : stageButtons)
		button->Render();
}

void StageSelectScene::PostRender()
{
}

void StageSelectScene::Start()
{
	//현재 스테이지 클리어 정보 로드
	LoadCurStageInfor();

	Vector2 offset = Vector2(0, -150);

	// 모든 스테이지를 다 깼을 경우(curStageNum == 4) 예외 처리
	int targetIndex = curStageNum;
	// 만약 4(Hard 클리어)라면, 버튼은 3번(Hard)까지만 있으므로 3번에 고정하거나
	// 혹은 'All Clear' 이미지를 따로 띄워야 함. 일단 3번(Hard) 위치에 고정.
	if (targetIndex >= 4)
	{
		targetIndex = 3;
		// 여기에 "All Clear!" 텍스트 같은 걸 띄우는 코드를 추가해도 좋음
	}

	curStage->SetPos(stageButtons[targetIndex]->GetPos() + offset);
	curStage->UpdateWorld();

	// 잠금 해제 로직: curStageNum만큼 버튼 활성화
	// (curStageNum이 4면 0,1,2,3 다 켜짐 -> 정상)
	for (int i = 0; i < 4; i++)
	{
		if (i <= curStageNum)
			stageButtons[i]->SetActive(true);
		else
			stageButtons[i]->SetActive(false); // 안 깬 건 확실히 끄기
	}

	/*for (int i = 0; i <= curStageNum; i++)
		stageButtons[i]->SetActive(true);*/
}

void StageSelectScene::OnClickStageButton(int stageNum)
{
	Audio::Get()->Stop("BGM");
	Audio::Get()->Play("ButtonClick_Select");

	string stageStr = "Stage" + to_string(stageNum);

	SceneManager::Get()->ChangeScene(stageStr);

	//튜토리얼인 경우에만 실행
	if (stageNum == 0)
	{
		((TutorialUI*)(UIManager::Get()->GetUIPanel("Tutorial")))->SetCurPage(0);
		((TutorialUI*)(UIManager::Get()->GetUIPanel("Tutorial")))->SetOn(true);
	}

	//카메라 기능 켜기
	CAM->CameraOn();
}

void StageSelectScene::LoadCurStageInfor()
{
	//정보 로드하기 - 현재 깨야하는 스테이지 번호
	BinaryReader* reader = new BinaryReader("TextData/SpiderGame/CurStage.srt");

	if (reader->IsFailed())
		return;

	curStageNum = reader->Int();

	delete reader;
}


//void StageSelectScene::OnEnteredStageButton()
//{
//	if(Audio::Get()->IsPlaySound("ButtonEnter_Select")) return;
//
//	//버튼위에 마우스가 올라왔을 때 소리 재생
//	for (Button* button : stageButtons)
//	{
//		if (button->IsPointCollision(mousePos))
//		{
//			Audio::Get()->Play("ButtonEnter_Select");
//			return;
//		}
//	}
//}
