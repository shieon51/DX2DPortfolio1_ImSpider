#pragma once

class UIManager : public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	bool IsContain(string strName, Vector2 curPos); // 현재 해당 UI가 활성되어있고 해당 영억을 클릭하고 있다면 ->뒤에 입력 안되도록

	class UIPanel* GetUIPanel(string strName) { return panels[strName]; }
	void AddUIPanel(string strName, class UIPanel* panel) { panels[strName] = panel; }

private:
	map<string, class UIPanel*> panels;

};