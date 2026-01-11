#pragma once

class EffectManager : public Singleton<EffectManager>
{
private:
    friend class Singleton;

    EffectManager() = default;
    ~EffectManager() = default;

public:
    void Update();
    void Render();

    void Add(string key, UINT poolSize, wstring textureFile,
        Vector2 maxFrame, float speed = 1.0f, bool isAdditive = false);

    void Play(string key, const Vector2& pos);

private:
    typedef vector<Effect*> Effects;
    map<string, Effects> totalEffect;
};