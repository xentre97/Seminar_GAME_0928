#pragma once
#include "MoveComponent.h"

/// <summary>
/// Playerの移動を司る
/// PlayerのStateを変更したりもする
/// </summary>
class PlayerControl :
    public MoveComponent
{
public:
    PlayerControl(class PlayerActor* owner);
    void input()override;
    void update()override;
    void fixFloorCol();

    float getAttackTime() const { return mAttackTime; }

private:
    void StateDraw(); // デバッグ用の関数
    class PlayerActor* mPlayer; // mOwnerと同じ(型が違う)
    // 攻撃用 これは無くすかも
    float mAttackTimer;
    float mAttackTime;  // 通常攻撃の時間
    // ため攻撃用
    float mChargeTimer;
    float mChargeTime;
    // 回避用
    float mLastPressTimeD;
    float mLastPressTimeA;
    float mDodgeTime;
    float mDodgeTimer;
    float mDodgeSpeed;
};
