#include "EnemyActor.h"
#include "GamePlay.h"
// Component
#include "SpriteComponent.h"
#include "EnemyMove.h"
#include "HpComponent.h"
#include "ArrowComponent.h"

EnemyActor::EnemyActor(Sequence* sequence)
	: Actor(sequence, Actor::Eenemy)
	, mEnemyState(E_walk)
{
	Texture2D* tex = mSequence->getTexture("testPlayerIdle.png");
	mPosition = Vector2{ 500.0f, 100.0f };
	mRectangle = {
		mPosition.x - tex->width / 2.0f,
		mPosition.y - tex->height / 2.0f,
		(float)tex->width,
		(float)tex->height
	};

	static_cast<GamePlay*>(mSequence)->addEnemy(this);

	// SpriteComponent
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->setTexture(*tex);
	// EnemyMove
	mEnemyMove = new EnemyMove(this);
	mEnemyMove->setMoveSpped(100.0f);
	// HpComponent
	mHpComp = new HpComponent(this, 200.0f); //仮に最大Hpを定数で決める。
	// WeaponComponent(Arrowにしてみた)
	mWeaponComp = new ArrowComponent(this);
}

EnemyActor::~EnemyActor()
{
	static_cast<GamePlay*>(mSequence)->removeEnemy(this);
}

void EnemyActor::update()
{
	Actor::update();

	//mHpComp->update();
}

void EnemyActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mSpriteComp->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mSpriteComp->getTexHeight() / 2.0f;
}

void EnemyActor::jump()
{
	if (mEnemyState == E_walk) {
		changeState(E_jump);
		mEnemyMove->setVelocityY(mEnemyMove->getJumpSpeed());
	}
}

void EnemyActor::changeState(EnemyState nextState)
{
	onExitState(nextState);
	onEnterState(nextState);
}

void EnemyActor::onEnterState(EnemyState nextState)
{
	// ステート変更
	mEnemyState = nextState;
	switch (mEnemyState)
	{
	case E_walk:
		break;
	case E_jump:
		break;
	case E_attack:
		mWeaponComp->startAttack();
		break;
	}
}

void EnemyActor::onExitState(EnemyState nextState)
{
	switch (mEnemyState)
	{
	case E_walk:
		break;
	case E_jump:
		break;
	case E_attack:
		mWeaponComp->endAttack();
		break;
	}
}