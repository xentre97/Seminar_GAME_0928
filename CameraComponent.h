#pragma once
#include "Component.h"
#include <raylib.h>

/// <summary>
/// ƒJƒƒ‰‚ÌŠî’ê(‚±‚Ì‚Ü‚Üg‚Á‚Ä‚¢‚é‚ª)
/// </summary>
class CameraComponent :
    public Component
{
public:
    CameraComponent(class Actor* owner);
    void update() override;
    const Camera2D& getCamera() const { return mCamera; }
private:
    Camera2D mCamera;
    float mStageWidth;
    float mStageHeight;
};

