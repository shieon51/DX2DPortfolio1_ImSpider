#include "Framework.h"

Effect::Effect(wstring textureFile, Vector2 maxFrame, float speed, bool isAdditive)
    : Quad(textureFile, Vector2(), Vector2(1 / maxFrame.x, 1 / maxFrame.y)),
    speed(speed), isAdditive(isAdditive), maxFrame(maxFrame)
{
    SetActive(false);

    maxFrameNum = (UINT)(maxFrame.x * maxFrame.y);
    material->SetMaxFrame(maxFrame);
}

void Effect::Update()
{
    if (!IsActive()) return;

    frameTime += DELTA;

    if (frameTime >= delayTime)
    {
        frameTime -= delayTime;

        curFrameNum++;

        curFrame.x = curFrameNum % (int)maxFrame.x;
        curFrame.y = curFrameNum / (int)maxFrame.x;

        material->SetCurFrame(curFrame);

        if (curFrameNum >= maxFrameNum)
            End();
    }

    UpdateWorld();
}

void Effect::Render()
{
    if (isAdditive)
        Environment::Get()->SetAdditiveBlend();

    Quad::Render();

    Environment::Get()->SetAlphaBlend();
}

void Effect::Play(const Vector2& pos)
{
    SetActive(true);
    SetPos(pos);

    frameTime = 0.0f;
    curFrameNum = 0;

    material->SetCurFrame(Vector2());
}

void Effect::End()
{
    SetActive(false);
}
