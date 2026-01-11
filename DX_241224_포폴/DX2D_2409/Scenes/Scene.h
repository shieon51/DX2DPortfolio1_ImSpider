#pragma once

class Scene
{
public:
    virtual void Update() = 0;

    virtual void PreRender() {}
    virtual void Render() = 0;
    virtual void PostRender() = 0;

    virtual void Start() {}
    virtual void End() {}
};