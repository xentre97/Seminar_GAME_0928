#include "Boss.h"
#include "GamePlay.h"

Boss::Boss(GamePlay* sequence)
	: EnemyActor(sequence)
	, mGamePlay(sequence)
{
}
