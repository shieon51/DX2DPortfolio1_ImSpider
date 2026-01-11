#include "Framework.h"

Camera::Camera()
{
    tag = "Camera";

    Init();

    uiView = XMMatrixIdentity();
}

Camera::~Camera()
{
    viewProjectionBuffer->Release();
}

void Camera::Update()
{
    if (target)
        FollowMode();
    else
        FreeMode();

    UpdateWorld();
}

void Camera::SetViewProjection()
{
    view = XMMatrixInverse(nullptr, world);
    viewProjection.view = XMMatrixTranspose(view);

    deviceContext->UpdateSubresource(viewProjectionBuffer, 0, nullptr, &viewProjection, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &viewProjectionBuffer);
}

void Camera::SetUIView()
{
    viewProjection.view = uiView;

    deviceContext->UpdateSubresource(viewProjectionBuffer, 0, nullptr, &viewProjection, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &viewProjectionBuffer);
}

void Camera::OffCameraLimitRange()
{
    isSetLimit = false;
    XLimit = Vector2();
    YLimit = Vector2();
}

void Camera::SetCameraLimitRange(float XMin, float XMax, float YMin, float YMax)
{
    isSetLimit = true;
    XLimit = { XMin, XMax };
    YLimit = { YMin, YMax };
}

void Camera::CameraOff() //카메라 기능 끄기
{
    target = nullptr;
    pos = Vector2();

    cameraOn = false;
}

void Camera::CameraOn() //카메라 기능 켜기
{
    cameraOn = true;
}

Vector2 Camera::ScreenToWorld(const Vector2& screenPos) //
{
    return pos + screenPos;
}

Vector2 Camera::WorldToScreen(const Vector2& worldPos) //
{
    return worldPos - pos;
}

void Camera::Init()
{
    {
        //ConstBuffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(ViewProjection);
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        device->CreateBuffer(&bufferDesc, nullptr, &viewProjectionBuffer);
    }

    viewProjection.view = XMMatrixIdentity();
    viewProjection.projection = XMMatrixOrthographicOffCenterLH(0, SCREEN_WIDTH,
        0, SCREEN_HEIGHT, -1, 1);

    viewProjection.projection = XMMatrixTranspose(viewProjection.projection);    
}

void Camera::FreeMode()
{
    if (!cameraOn) return;

    if (KEY->Press(VK_RBUTTON))
    {
        if (isSetLimit)
        {
            if (pos.y + SCREEN_HEIGHT < YLimit.y && KEY->Press('W'))
                Translate(Vector2::Up() * freeModeSpeed * DELTA);
            if (pos.y > YLimit.x && KEY->Press('S'))
                Translate(Vector2::Down() * freeModeSpeed * DELTA);
            if (pos.x > XLimit.x && KEY->Press('A'))
                Translate(Vector2::Left() * freeModeSpeed * DELTA);
            if (pos.x + SCREEN_WIDTH < XLimit.y && KEY->Press('D'))
                Translate(Vector2::Right() * freeModeSpeed * DELTA);
        }
        else
        {
            if (KEY->Press('W'))
                Translate(Vector2::Up() * freeModeSpeed * DELTA);
            if (KEY->Press('S'))
                Translate(Vector2::Down() * freeModeSpeed * DELTA);
            if (KEY->Press('A'))
                Translate(Vector2::Left() * freeModeSpeed * DELTA);
            if (KEY->Press('D'))
                Translate(Vector2::Right() * freeModeSpeed * DELTA);
        }
        
    }
}

void Camera::FollowMode()
{
    if (!cameraOn) return;

    offset = CENTER + Vector2(0, -100);
    destPos = target->GetGlobalPos() - offset;
    pos = GameMath::Lerp<Vector2>(pos, destPos, followModeSpeed * DELTA);
}
