#pragma once

class GameResultUI : public UIPanel
{
public:
	GameResultUI();
	~GameResultUI();

	void Update();
	void PanelVisiable() override;
	void PrintGameClear();
	void PrintGameOver();

	void OnClickButton();

private:
	vector<Button*> buttons;
	Quad* imageWin;
	Quad* imageLose;

	bool isGameClear = true;
};