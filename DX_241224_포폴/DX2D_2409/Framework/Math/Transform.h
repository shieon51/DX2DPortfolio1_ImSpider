#pragma once

class Transform
{
public:
    Transform();
    
    void UpdateWorld();
    void Edit();

    void Translate(float x, float y);
    void Translate(Vector2 velocity);
    void Rotate(float velocity);

    void Save();
    void Load();

    Vector2 GetPos() const { return pos; }
    Float3 GetRot() const { return rot; }
    Vector2 GetScale() const { return scale; }
    float GetAngle() const { return rot.z; }

    Vector2 GetUp() const { return up.Normalized(); }
    Vector2 GetDown() const { return GetUp() * -1.0f; }
    Vector2 GetRight() const { return right.Normalized(); }
    Vector2 GetLeft() const { return GetRight() * -1.0f; }

    Vector2 GetGlobalPos() const { return globalPos; }
    Vector2 GetGlobalScale() const { return globalScale; }

    void SetPos(Vector2 pos) { this->pos = pos; }
    void SetScale(Vector2 scale) { this->scale = scale; }
    void SetRot(Float3 rot) { this->rot = rot; }

    void SetPos(float x, float y) { pos = { x, y }; }
    void SetScale(float x, float y) { scale = { x, y }; }
    void SetRot(float x, float y, float z) { rot = { x, y, z }; }

    //ÇÇº¿
    void SetPivot(Vector2 p) { pivot = p; }
    Vector2 GetPivot() { return pivot; }

    bool IsActive() const;
    void SetActive(bool isActive) { this->isActive = isActive; }

    void SetParent(Transform* parent) { this->parent = parent; }
    void SetTag(string tag) { this->tag = tag; }

    string GetTag() { return tag; }    
    Matrix GetWorld() { return world; }

protected:
    string tag;
    bool isActive = true;

    Matrix world;
    Matrix S, R, T, P, IP;

    Vector2 pos = {};
    Float3 rot = {};
    Vector2 scale = { 1, 1 };
    Vector2 pivot = { 0, 0 };

    Vector2 globalPos;
    Float4 globalRot;
    Vector2 globalScale;

    Vector2 up, right;

    XMFLOAT4X4 matWorld;

    Transform* parent = nullptr;
};