#pragma once

class Camera : public Transform
{
private:
    struct ViewProjection
    {
        Matrix view;
        Matrix projection;
    };

public:
    Camera();
    ~Camera();

    void Update();

    void SetViewProjection();
    void SetUIView();

    void SetTarget(Transform* target) { this->target = target; }

    void OffCameraLimitRange(); 
    void SetCameraLimitRange(float XMin, float XMax, float YMin, float YMax);
    void CameraOff();
    void CameraOn();

    Vector2 ScreenToWorld(const Vector2& screenPos);
    Vector2 WorldToScreen(const Vector2& worldPos);

private:
    void Init();

    void FreeMode();
    void FollowMode();

private:
    ViewProjection viewProjection;
    ID3D11Buffer* viewProjectionBuffer;

    Matrix view, uiView;

    float freeModeSpeed = 300.0f;
    float followModeSpeed = 8.0f;

    Transform* target = nullptr;

    Vector2 destPos;
    Vector2 offset = CENTER;

    bool cameraOn = true;
    bool isSetLimit = false;
    Vector2 XLimit;
    Vector2 YLimit;
};