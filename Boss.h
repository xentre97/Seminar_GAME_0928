#pragma once
#include "EnemyActor.h"

/// <summary>
/// É{ÉX
/// </summary>
class Boss :
    public EnemyActor
{
public:
    Boss(class GamePlay* sequence);

private:
    class GamePlay* mGamePlay;
};

