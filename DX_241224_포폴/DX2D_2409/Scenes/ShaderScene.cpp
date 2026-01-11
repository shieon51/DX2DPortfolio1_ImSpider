#include "Framework.h"
#include "ShaderScene.h"

ShaderScene::ShaderScene()
{
    quad = new Quad(L"Textures/Test.jpg");
    quad->SetPos(CENTER);
    quad->UpdateWorld();
    quad->GetMaterial()->SetShader(L"Shaders/Multi.hlsl");

    secondMap = Texture::Add(L"Textures/Sun.png");

    intValueBuffer = new IntValueBuffer();
    intValueBuffer->Get()[2] = quad->GetSize().x;
    intValueBuffer->Get()[3] = quad->GetSize().y;

    floatValueBuffer = new FloatValueBuffer();    
}

ShaderScene::~ShaderScene()
{
    delete quad;
    delete intValueBuffer;
}

void ShaderScene::Update()
{
    //static float temp = 0.0f;
    //temp += DELTA;
    //quad->GetMaterial()->SetCurFrame(temp, 0);
}

void ShaderScene::Render()
{
    intValueBuffer->SetPS(2);
    floatValueBuffer->SetPS(3);

    secondMap->PSSet(1);

    quad->Render();
}

void ShaderScene::PostRender()
{
    quad->GetMaterial()->Edit();
    ImGui::DragInt("Select", &intValueBuffer->Get()[0]);
    ImGui::DragInt("Scale", &intValueBuffer->Get()[1]);

    ImGui::DragFloat("Scale1", &floatValueBuffer->Get()[0]);
    ImGui::DragFloat("Scale2", &floatValueBuffer->Get()[1]);
    ImGui::DragFloat("Scale3", &floatValueBuffer->Get()[2]);
}
