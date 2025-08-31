#include "SwordActor.h"
#include "GamePlay.h"

#include "AnimSpriteComponent.h"


SwordActor::SwordActor(Sequence* sequence, Type type, Type owner)
	: WeaponActor(sequence, type, owner)
{
	Texture2D* tex = mSequence->getTexture("testSword01.png");
	mPosition = Vector2{ 500.0f, 300.0f };
	mRectangle = {
		mPosition.x - tex->width / 2.0f,
		mPosition.y - tex->height / 2.0f,
		(float)tex->width,
		(float)tex->height
	};

	mAnimsc = new AnimSpriteComponent(this);
	std::vector<Texture2D*> texs = {
		// モーション1
		tex,
		mSequence->getTexture("testSword02.png"),
		mSequence->getTexture("testSword03.png"),
		mSequence->getTexture("testSword04.png"),
		mSequence->getTexture("testSword05.png"),
		mSequence->getTexture("testSword06.png"),
		mSequence->getTexture("testSword07.png"),
		mSequence->getTexture("testSword08.png"),
		mSequence->getTexture("testSword09.png"),
		mSequence->getTexture("testSword10.png"),
		// モーション2
		mSequence->getTexture("testSword07.png"),
		mSequence->getTexture("testSword08.png"),
		mSequence->getTexture("testSword09.png"),
		mSequence->getTexture("testSword10.png"),
	};
	mAnimsc->setAnimTextures(texs);
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

void SwordActor::onStartAttack(int begin, int end, float attackTime)
{
	mAnimsc->play(begin, end, false, (end - begin) / attackTime);
}

void SwordActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}
