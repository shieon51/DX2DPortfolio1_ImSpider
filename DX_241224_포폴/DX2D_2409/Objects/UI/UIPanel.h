#pragma once

class UIPanel : public Quad
{
public:
	UIPanel();
	~UIPanel();

	virtual void PanelVisiable() = 0;

	void SetOn(bool isOn) { this->isOn = isOn; }
	bool IsOn() { return isOn; }

protected:
	bool isOn = false; //해당 UI창이 떠있는지
};