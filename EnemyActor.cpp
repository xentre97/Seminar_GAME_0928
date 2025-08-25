#include "EnemyActor.h"
#include "GamePlay.h"
// Component
#include "SpriteComponent.h"
#include "EnemyMove.h"

EnemyActor::EnemyActor(Sequence* sequence, Type type)
	: Actor(sequence, type)
{
	Texture2D tex = mSequence->getTexture("testPlayerIdle.png");
	mPosition = Vector2{ 500.0f, 100.0f };
	mRectangle = {
		mPosition.x - tex.width / 2.0f,
		mPosition.y - tex.height / 2.0f,
		(float)tex.width,
		(float)tex.height
	};

	static_cast<GamePlay*>(mSequence)->addEnemy(this);
	
	// SpriteComponent
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->setTexture(tex);
	// EnemyMove
	mEnemyMove = new EnemyMove(this);
	mEnemyMove->setMoveSpped(100.0f);
}

EnemyActor::~EnemyActor()
{
	static_cast<GamePlay*>(mSequence)->removeEnemy(this);
}

void EnemyActor::update()
{
	Actor::update();
}

void EnemyActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mSpriteComp->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mSpriteComp->getTexHeight() / 2.0f;
}

void EnemyActor::jump()
{
	if (mEnemyMove) {
		mEnemyMove->setVelocityY(mEnemyMove->getJumpSpeed());
	}
}

EnemyMove& EnemyActor::getEnemyMove()
{
	if (mEnemyMove) {
		return *mEnemyMove;
	}
}
