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
	//void SetIsGameClear(bool result) { isGameClear = result; }

private:
	vector<Button*> buttons;

	bool isGameClear = true;
};