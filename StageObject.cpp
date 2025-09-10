#include "StageObject.h"
#include "GamePlay.h"

#include "SpriteComponent.h"
#include "HpComponent.h"

#include "ExplosionActor.h"

StageObject::StageObject(Sequence* sequence)
	: Actor(sequence, Type::EstageObject)
{
	static_cast<GamePlay*>(mSequence)->addStageObj(this);

	Texture2D* tex = mSequence->getTexture("testWoodenBoard.png");
	mRectangle = {
		mPosition.x - tex->width / 2.0f,
		mPosition.y - tex->height / 2.0f,
		(float)tex->width,
		(float)tex->height
	};
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->setTexture(*tex);
	mHpComp = new HpComponent(this, 1.0f);
}

StageObject::~StageObject()
{
	static_cast<GamePlay*>(mSequence)->removeStageObj(this);
}

void StageObject::update()
{
	Actor::update();

	if (mHpComp->IsKilled()) {
		setState(Edead);
	}
}

void StageObject::computeRectangle()
{
	mRectangle.x = mPosition.x - mSpriteComp->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mSpriteComp->getTexHeight() / 2.0f;
}
