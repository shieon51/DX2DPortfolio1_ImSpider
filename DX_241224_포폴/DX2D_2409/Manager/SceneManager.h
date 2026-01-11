#pragma once

class SceneManager : public Singleton<SceneManager>
{
private:
    friend class Singleton;

    SceneManager() = default;
    ~SceneManager();

public:
    void Update();

    void PreRender();
    void Render();
    void PostRender();

    void Add(string key, Scene* scene);

    void ChangeScene(string key);

    //Scene* GetCurScene() { return curScene; }

private:
    map<string, Scene*> scenes;

    Scene* curScene = nullptr;
};