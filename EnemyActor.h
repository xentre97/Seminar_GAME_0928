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
    EnemyActor(class Sequence* sequence, Type type);
    ~EnemyActor();

    void update() override;
    void computeRectangle() override;

    void jump();

    class EnemyMove& getEnemyMove();

private:
    class SpriteComponent* mSpriteComp;
    class EnemyMove* mEnemyMove;
};

