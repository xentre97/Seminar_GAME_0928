#pragma once
#include "MoveComponent.h"

/// <summary>
/// �G�̈ړ��������N���X
/// </summary>
class EnemyMove :
    public MoveComponent
{
public:
    EnemyMove(class EnemyActor* owner);

    void update()override;
    void fixFloorCol();

private:
    class EnemyActor* mEnemy;   // mOwner�Ɠ���(cast�ȗ��p)
    class Actor* mTarget;
    float mAttackRange;
    float mAttackTime;
    float mAttackTimer;
};

