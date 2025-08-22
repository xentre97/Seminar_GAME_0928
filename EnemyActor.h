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
    EnemyActor(class Sequence* sequence);
    ~EnemyActor();

    void update() override;
};

