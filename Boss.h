#pragma once
#include "EnemyActor.h"

/// <summary>
/// �{�X
/// </summary>
class Boss :
    public EnemyActor
{
public:
    Boss(class GamePlay* sequence);

private:
    class GamePlay* mGamePlay;
};

