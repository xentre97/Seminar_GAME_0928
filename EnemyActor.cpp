#include "EnemyActor.h"
#include "GamePlay.h"
// Component
#include "AnimSpriteComponent.h"
#include "HpComponent.h"

EnemyActor::EnemyActor(Sequence* sequence)
	: Actor(sequence, Actor::Eenemy)
{
	static_cast<GamePlay*>(mSequence)->addEnemy(this);

	mAnimsc = new AnimSpriteComponent(this);
	mHpComp = new HpComponent(this, 20.0f);
}

EnemyActor::~EnemyActor()
{
	static_cast<GamePlay*>(mSequence)->removeEnemy(this);
}
