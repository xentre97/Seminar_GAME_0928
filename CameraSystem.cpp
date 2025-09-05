#include "CameraSystem.h"

CameraSystem::CameraSystem(float stageWidth)
    : mStageWidth(stageWidth)
    , mMode(Mode::FollowPlayer)
    , mPlayer(nullptr)
{
    mCamera.zoom = 1.0f;
    mCamera.rotation = 0.0f;
    mCamera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    mFixedTarget = { stageWidth / 2.0f, GetScreenHeight() / 2.0f };
}

void CameraSystem::update() {
    switch (mMode) {
    case Mode::FollowPlayer:
        if (mPlayer) followPlayer();
        break;
    case Mode::Fixed:
        mCamera.target = mFixedTarget;
        break;
    }
}

void CameraSystem::followPlayer() {
    float centerX = GetScreenWidth() / 2.0f;
    float targetX = mPlayer->getPosition().x;

    if (targetX < centerX) targetX = centerX;
    if (targetX > mStageWidth - centerX) targetX = mStageWidth - centerX;

    mCamera.target = { targetX, GetScreenHeight() / 2.0f };
}
