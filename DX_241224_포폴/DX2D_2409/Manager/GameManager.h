#pragma once

class GameManager
{
public:
    GameManager();
    ~GameManager();

    void Update();

    void PreRender();
    void Render();

private:
    void Init();
};