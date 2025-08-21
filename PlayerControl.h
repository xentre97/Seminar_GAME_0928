#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player‚ÌˆÚ“®‚ðŽi‚é
/// Action‚à‚¢‚ê‚Ä‚é
/// </summary>
class PlayerControl :
    public MoveComponent
{
public:
    enum MoveState
    {
        ms_idle,
        ms_walk,
        ms_dash,
        ms_jump,
    };
    enum ActionState
    {
        as_idle,
        as_attack,
        as_guard
    };
    PlayerControl(class PlayerActor* actor);
    void input()override;
    void update()override;
    void toIdle() { mMoveState = ms_idle; }
    void fixFloorCol();

private:
    void StateDraw();
    class PlayerActor* mPlayer; // mOwner‚Æ“¯‚¶(Œ^‚ªˆá‚¤)
    MoveState mMoveState;
    ActionState mActionState;
    bool mIsGuarding;
    float mDashSpeed;
    float actionTimer;
};

//class MoveState
//{
//public:
//    enum state
//    {
//        ms_idle,
//        ms_walk,
//        ms_dash,
//        ms_jump,
//    };
//    void input();
//    void update();
//    void changeState();
//
//    state mMoveState;
//};
//
//class ActionState
//{
//public:
//    enum state
//    {
//        as_idle,
//        as_attack,
//        as_guard
//    };
//    void input();
//    void update();
//    void changeState();
//    
//    state mActionState;
//private:
//
//};