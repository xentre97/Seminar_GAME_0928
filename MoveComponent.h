#pragma once
#include "Component.h"

/// <summary>
/// PlayerControl,EnemyMoveの基底
/// </summary>
class MoveComponent :
    public Component
{
public:
    MoveComponent(class Actor* owner);
    void update() override {} ;

    // 以下一応用意
    float getMoveSpeed() const { return mMoveSpeed; }
    float getJumpSpeed() const { return mJumpSpeed; }
    float getVelocityY() const { return mVelocityY; }
    bool isJumping() const { return mIsJumping; }
    void setMoveSpped(float speed) { mMoveSpeed = speed; }
    void setJumpSpped(float speed) { mJumpSpeed = speed; }
    void setVelocityY(float velocity) { mVelocityY = velocity; }
    void setJumping(bool jumping) { mIsJumping = jumping; }

protected:
    float mGravity;     // 重力
    float mMoveSpeed;   // 横方向の移動速度
    float mVelocityX;   // 横方向の速度
    float mJumpSpeed;   // ジャンプの初速度
    float mVelocityY;   // 縦方向の速度
    bool mIsJumping;
};

