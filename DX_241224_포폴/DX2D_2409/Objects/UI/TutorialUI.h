#pragma once

class TutorialUI : public UIPanel
{
public: 
	TutorialUI();
	~TutorialUI();

	void PanelVisiable() override;
	void NextPage() { curPage++; }
	int GetCurPage() { return curPage; }
	void SetCurPage(int page) { curPage = page; }

private:
	vector<Quad*> tutorialInfors;

	//튜토리얼 현재 페이지
	int curPage = 0;
};