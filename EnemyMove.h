#pragma once
#include "MoveComponent.h"

/// <summary>
/// “G‚ÌˆÚ“®‚ğˆµ‚¤ƒNƒ‰ƒX
/// </summary>
class EnemyMove :
    public MoveComponent
{
public:
    EnemyMove(class EnemyActor* owner);

    void update()override;
    void fixFloorCol();

private:
    class EnemyActor* mEnemy;   // mOwner‚Æ“¯‚¶(castÈ—ª—p)
    class Actor* mTarget;
    float mAttackRange;
};

