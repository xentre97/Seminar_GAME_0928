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
    // enumを分けていない為,mActionState = ms_idle は可能,気を付ける
    // ステート毎の処理が多い為,enumではなくclassにした方がいいと思う
    // クラスにすればそっちにテクスチャを持たせたり,AttackTimeのような,ある状態でしか
    // 使わない変数をある状態のみに押し込めることができる
    enum PlayerState
    {
        // moveState
        ms_idle,
        ms_walk,
        ms_jump,    // ジャンプというよりも空中にいるステートという感じ
        // actionState
        as_idle,
        as_attack,
        as_guard,
        as_charge,
        as_dodge, // 回避
    };
    PlayerActor(class Sequence* sequence, Type type);

    void input() override;
    void update() override;

    const Camera2D& getCamera() const ;
    class PlayerControl& getPlayerControl();
    class SwordComponent* getWeapon() const { return mSwordComp; }
    PlayerState getMoveState() { return mMoveState; }
    PlayerState getActionState() { return mActionState; }
    
    void computeRectangle() override;
    void changeState(PlayerState nextState);

private:
    void onEnterState(PlayerState nextState);
    void onExitState(PlayerState nextState);
    
    //std::vector<Texture2D*>
    PlayerState mMoveState;
    PlayerState mActionState;
    class CameraComponent* mCameraComp;
    class PlayerControl* mPlayerControl;
    class AnimSpriteComponent* mAnimsc;
    class SwordComponent* mSwordComp;
    class ArrowComponent* mArrowComp;
};

// 挙動
// AD : 移動
// Space : ジャンプ
// Shift + 移動 : ダッシュ
// S : ガード(移動,ジャンプ不可)
// W : 攻撃
