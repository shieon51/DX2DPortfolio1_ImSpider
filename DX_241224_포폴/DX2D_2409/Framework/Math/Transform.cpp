#include "Framework.h"

Transform::Transform()
{
    world = XMMatrixIdentity();
}

void Transform::UpdateWorld()
{
    S = XMMatrixScaling(scale.x, scale.y, 1);
    R = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    T = XMMatrixTranslation(pos.x, pos.y, 0);
    P = XMMatrixTranslation(pivot.x, pivot.y, 0);
    IP = XMMatrixInverse(nullptr, P);    

    world = IP * S * R * T * P;

    if (parent)
        world *= parent->world;//world = world * parnet->world;   

    XMStoreFloat4x4(&matWorld, world);
    
    XMVECTOR S, R, T;
    XMMatrixDecompose(&S, &R, &T, world);

    XMStoreFloat2(&globalScale, S);
    XMStoreFloat2(&globalPos, T);

    right = { matWorld._11, matWorld._12 };
    up = { matWorld._21, matWorld._22 };
}

void Transform::Edit()
{
    if (ImGui::TreeNode((tag + "_Transform").c_str()))
    {
        ImGui::Checkbox("Active", &isActive);

        ImGui::DragFloat2("Pos", (float*)&pos, 1.0f);

        Float3 tempRot;
        tempRot.x = XMConvertToDegrees(rot.x);
        tempRot.y = XMConvertToDegrees(rot.y);
        tempRot.z = XMConvertToDegrees(rot.z);

        ImGui::DragFloat3("Rot", (float*)&tempRot, 0.1f, -180.0f, 180.0f);

        rot.x = XMConvertToRadians(tempRot.x);
        rot.y = XMConvertToRadians(tempRot.y);
        rot.z = XMConvertToRadians(tempRot.z);

        ImGui::DragFloat2("Scale", (float*)&scale, 0.1f);

        if (ImGui::Button("Save"))
            Save();
        
        if (ImGui::Button("Load"))
            Load();
        
        ImGui::TreePop();
    }
}

void Transform::Translate(float x, float y)
{
    pos.x += x;
    pos.y += y;
}

void Transform::Translate(Vector2 velocity)
{
    pos += velocity;
}

void Transform::Rotate(float velocity)
{
    rot.z += velocity;
}

void Transform::Save()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/" + tag + ".srt");

    writer->Vector(pos);

    writer->Float(rot.x);
    writer->Float(rot.y);
    writer->Float(rot.z);

    writer->Vector(scale);

    delete writer;
}

void Transform::Load()
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".srt");

    if (reader->IsFailed())
        return;

    pos = reader->Vector();

    rot.x = reader->Float();
    rot.y = reader->Float();
    rot.z = reader->Float();

    scale = reader->Vector();

    delete reader;
}

bool Transform::IsActive() const
{
    if (parent == nullptr)
        return isActive;

    if (isActive == false)
        return false;

    return parent->IsActive();
}
