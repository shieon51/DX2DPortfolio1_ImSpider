#include "Framework.h"

Action::Action(string path, string file, bool isLoop, float speed)
{
    LoadClip(path, file, isLoop, speed);
}

Action::~Action()
{
    for (Clip* clip : clips)
        delete clip;
}

void Action::Update()
{
    clips[curState]->Update();
}

void Action::Render()
{
    clips[curState]->Render();
}

void Action::Start()
{
    clips[curState]->Play();
}

void Action::SetShader(wstring shaderFile)
{
    for (Clip* clip : clips)
    {
        clip->SetShader(shaderFile);
    }
}

void Action::LoadClip(string path, string file, bool isLoop, float speed)
{
    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
    document->LoadFile((path + file).c_str());

    tinyxml2::XMLElement* atlas = document->FirstChildElement();
    string textureFile = path + atlas->Attribute("imagePath");

    vector<Frame*> frames;
    tinyxml2::XMLElement* sprite = atlas->FirstChildElement();

    while (sprite != nullptr)
    {
        float x, y, w, h;
        x = sprite->FloatAttribute("x");
        y = sprite->FloatAttribute("y");
        w = sprite->FloatAttribute("w");
        h = sprite->FloatAttribute("h");

        frames.push_back(new Frame(Utility::ToWString(textureFile), x, y, w, h));

        sprite = sprite->NextSiblingElement();
    }

    clips.push_back(new Clip(frames, isLoop, speed));    

    delete document;
}

void Action::LoadClip(wstring file, int frameX, int frameY, bool isLoop, float speed)
{
    vector<Frame*> frames;

    Vector2 uvSize = Vector2(1.0f / frameX, 1.0f / frameY);

    for (int y = 0; y < frameY; y++)
    {
        for (int x = 0; x < frameX; x++)
        {
            Vector2 startUV = uvSize * Vector2(x, y);
            Vector2 endUV = startUV + uvSize;

            frames.push_back(new Frame(file, startUV, endUV));
        }
    }

    clips.push_back(new Clip(frames, isLoop, speed));
}

void Action::SetState(int state)
{
    if (curState == state) return;

    curState = state;
    clips[state]->Play();
}
