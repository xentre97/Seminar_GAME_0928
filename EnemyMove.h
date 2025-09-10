#pragma once
#include "MoveComponent.h"

/// <summary>
/// “G‚ÌˆÚ“®‚ğˆµ‚¤ƒNƒ‰ƒX
/// </summary>
class EnemyMove :
    public MoveComponent
{
public:
    EnemyMove(class WeakEnemy* owner);

    void update()override;
    void fixFloorCol();
    void setAttackRange(float range) { mAttackRange = range; }

private:
    class WeakEnemy* mEnemy;   // mOwner‚Æ“¯‚¶(castÈ—ª—p)
    class Actor* mTarget;
    float mAttackRange;
    float mAttackTime;
    float mAttackTimer;
};

