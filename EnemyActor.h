#pragma once
#include "Actor.h"

/// <summary>
/// 敵アクター
/// 必要に応じてまた継承する
/// </summary>
class EnemyActor :
    public Actor
{
public:
    virtual ~EnemyActor();
    class HpComponent* getHpComp() { return mHpComp; }

protected:
    EnemyActor(class Sequence* sequence);
    class HpComponent* mHpComp;
    class AnimSpriteComponent* mAnimsc;
};

