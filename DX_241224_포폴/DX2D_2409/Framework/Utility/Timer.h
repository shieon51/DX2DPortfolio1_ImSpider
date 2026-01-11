#pragma once

class Timer : public Singleton<Timer>
{
private:
    friend class Singleton;

    Timer();
    ~Timer();

public:
    void Update();

    float GetElapsedTime() { return elapsedTime > 0.01f ? 0.01f : elapsedTime; }
    int GetFPS() { return frameRate; }

private:
    float timeScale = 0.0f;
    float elapsedTime = 0.0f;
    float oneSecCount = 0.0f;

    __int64 curTime = 0;
    __int64 lastTime = 0;
    __int64 periodFrequency = 0;

    int frameRate = 0;
    int frameCount = 0;
};