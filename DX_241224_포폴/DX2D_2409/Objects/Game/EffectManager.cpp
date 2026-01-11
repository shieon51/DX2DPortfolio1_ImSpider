#include "Framework.h"

void EffectManager::Update()
{
    for (pair<string, Effects> effects : totalEffect)
    {
        for (Effect* effect : effects.second)
            effect->Update();
    }
}

void EffectManager::Render()
{
    for (pair<string, Effects> effects : totalEffect)
    {
        for (Effect* effect : effects.second)
            effect->Render();
    }
}

void EffectManager::Add(string key, UINT poolSize, wstring textureFile, Vector2 maxFrame, float speed, bool isAdditive)
{
    Effects effects;

    for (UINT i = 0; i < poolSize; i++)
    {
        Effect* effect = new Effect(textureFile, maxFrame, speed, isAdditive);
        effects.push_back(effect);
    }

    totalEffect[key] = effects;
}

void EffectManager::Play(string key, const Vector2& pos)
{
    if (totalEffect.count(key) == 0)
        assert(false);

    for (Effect* effect : totalEffect[key])
    {
        if (!effect->IsActive())
        {
            effect->Play(pos);
            return;
        }
    }
}
