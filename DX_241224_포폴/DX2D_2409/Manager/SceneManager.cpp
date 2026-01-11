#include "Framework.h"

SceneManager::~SceneManager()
{
    //for (auto scene : scenes)
    for (pair<string, Scene*> scene : scenes)
    {
        delete scene.second;
    }
}

void SceneManager::Update()
{
    if (curScene == nullptr) return;

    curScene->Update();
}

void SceneManager::PreRender()
{
    if (curScene == nullptr) return;

    curScene->PreRender();
}

void SceneManager::Render()
{
    if (curScene == nullptr) return;

    curScene->Render();
}

void SceneManager::PostRender()
{
    if (curScene == nullptr) return;

    curScene->PostRender();
}

void SceneManager::Add(string key, Scene* scene)
{
    if (scenes.count(key) > 0)
    {
        assert(false);
    }

    //scenes.insert({ key, scene });
    //scenes.insert(make_pair(key, scene));
    //scenes.insert(pair<string, Scene*>(key, scene));

    scenes[key] = scene;
}

void SceneManager::ChangeScene(string key)
{
    if (scenes.count(key) == 0)
        assert(false);

    if(scenes[key] == curScene)
        assert(false);

    if (curScene != nullptr)
        curScene->End();

    curScene = scenes[key];
    curScene->Start();
}
