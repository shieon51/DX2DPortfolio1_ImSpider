#pragma once

class Effect : public Quad
{
private:
    const float FPS = 10.0f;

public:
    Effect(wstring textureFile, Vector2 maxFrame, float speed, bool isAdditive);

    void Update();
    void Render();

    void Play(const Vector2& pos);
    void End();

private:
    float delayTime = 1.0f / FPS;
    float frameTime = 0.0f;
    float speed;

    UINT curFrameNum = 0;
    UINT maxFrameNum = 0;

    Vector2 curFrame;
    Vector2 maxFrame;

    bool isAdditive;
};