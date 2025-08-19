#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player‚ÌˆÚ“®‚ðŽi‚é
/// </summary>
class PlayerMove :
    public MoveComponent
{
public:
    PlayerMove(class Actor* actor);
    void input()override;
    void update()override;
private:
};

