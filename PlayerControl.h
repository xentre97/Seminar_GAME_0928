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
    PlayerControl(class PlayerActor* actor);
    void input()override;
    void update()override;
    void fixFloorCol();

private:
    void StateDraw(); // デバッグ用の関数
    class PlayerActor* mPlayer; // mOwnerと同じ(型が違う)
    bool mIsGuarding;
    float mDashSpeed;
    float actionTimer;
};
