#pragma once
#include "Actor.h"
#include <map>
#include "PlayerControl.h"

/// <summary>
/// PlayerActorクラス
/// </summary>
class PlayerActor :
    public Actor
{
public:
    // 行動の状態とアクションの状態に分けた
    // それぞれの状態を表す変数がある(mMoveState,mActionState)
    // mActionState = ms_idle は可能,気を付ける必要がある
    // enumを分けてもいい
    enum PlayerState
    {
        // moveState
        ms_idle,
        ms_walk,
        ms_dash,
        ms_jump,
        // actionState
        as_idle,
        as_attack,
        as_guard
    };
    PlayerActor(class Sequence* sequence);

    void input() override;
    void update() override;

    const Camera2D& getCamera() const ;
    class PlayerControl& getPlayerControl();
    PlayerState getMoveState() { return mMoveState; }
    PlayerState getActionState() { return mActionState; }
    
    void computeRectangle();
    void setMoveState(PlayerState state) { mMoveState = state; }
    void setActionState(PlayerState state) { mActionState = state; }
    
private:
    PlayerState mMoveState;
    PlayerState mActionState;
    class CameraComponent* mCameraComp;
    class PlayerControl* mPlayerControl;
    class AnimSpriteComponent* mAnimsc;
    bool mForward;
};

// 挙動
// AD : 移動
// Space : ジャンプ
// Shift + 移動 : ダッシュ
// S : ガード(移動,ジャンプ不可)
// W : 攻撃

// 状態 状態によってアニメーションを変える
// 攻撃...1枚の絵
// 移動...ループアニメーション
// ジャンプ...ジャンプの絵
// ガード...一枚の絵
// 見てる向き(一旦保留)