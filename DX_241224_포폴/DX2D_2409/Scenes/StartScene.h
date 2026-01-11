#pragma once

class StartScene : public Scene
{
public:
	StartScene();
	~StartScene();

	void Update() override;
	void Render() override;
	void PostRender() override;

	void OnClickStartGameButton();
	void OnClickExitButton();

private:
	Button* startGameButton;
	Button* exitButton;

	Quad* background;
};