#include "EnemyActor.h"
#include "GamePlay.h"
#include "SpriteComponent.h"

EnemyActor::EnemyActor(Sequence* sequence)
	: Actor(sequence)
{
	Texture2D tex = mSequence->getTexture("test.png");
	mPosition = Vector2{ 500.0f, 300.0f };
	mRectangle = {
		mPosition.x - tex.width / 2.0f,
		mPosition.y - tex.height / 2.0f,
		(float)tex.width,
		(float)tex.height
	};

	static_cast<GamePlay*>(mSequence)->addEnemy(this);
	mSpriteComp = new SpriteComponent(this);
	mSpriteComp->setTexture(tex);
}

EnemyActor::~EnemyActor()
{
}
void EnemyActor::update()
{
	// 位置更新
	mPosition.x += GetRandomValue(-10, 10);
	mPosition.y += GetRandomValue(-10, 10);
	// 当たり判定用矩形の更新
	mRectangle.x = mPosition.x - mSpriteComp->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mSpriteComp->getTexHeight() / 2.0f;
}
