#include "SwordActor.h"
#include "GamePlay.h"

#include "AnimSpriteComponent.h"


SwordActor::SwordActor(Sequence* sequence, Type owner)
	: WeaponActor(sequence, owner)
	, mColWidth(64.0f)
	, mColHeight(32.0f)
{
	mPosition = Vector2{ 500.0f, 300.0f };
	mRectangle = {
		mPosition.x - mColWidth / 2.0f,
		mPosition.y - mColHeight / 2.0f,
		mColWidth,
		mColHeight
	};
}

void SwordActor::input()
{
	Actor::input();
}

void SwordActor::update()
{
	Actor::update();
	computeRectangle();
}

void SwordActor::onStartAttack()
{
}

void SwordActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mColWidth / 2.0f;
	mRectangle.y = mPosition.y - mColHeight / 2.0f;
}
