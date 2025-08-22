#include "EnemyActor.h"
#include "GamePlay.h"
#include "SpriteComponent.h"

EnemyActor::EnemyActor(Sequence* sequence)
	: Actor(sequence)
{
	mTexture = LoadTexture("test.png");
	mPosition = Vector2{ 500.0f, 300.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	static_cast<GamePlay*>(mSequence)->addEnemy(this);
	SpriteComponent* sc = new SpriteComponent(this);
	sc->setTexture(mTexture);
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
	mRectangle.x = mPosition.x - mTexture.width / 2.0f;
	mRectangle.y = mPosition.y - mTexture.height / 2.0f;
}
