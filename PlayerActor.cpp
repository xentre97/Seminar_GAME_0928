#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence)
{
	mTexture = LoadTexture("test.png");
	mPosition = Vector2{ 0.0f, 0.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	mCameraComp = new CameraComponent(this);
}

void PlayerActor::input()
{
	// 基底のinput() : Componentのinput
	// moveComponent的なのを作るなら意義を持ちそう。作らないなら無くてもいい
	Actor::input();
	// WASD : 平行移動する
	if (IsKeyDown(KEY_S)) {
		mPosition.y += 15;
	}
	if (IsKeyDown(KEY_W)) {
		mPosition.y -= 15;
	}
	if (IsKeyDown(KEY_A)) {
		mPosition.x -= 15;
	}
	if (IsKeyDown(KEY_D)) {
		mPosition.x += 15;
	}
}

void PlayerActor::update()
{
	// 基底のupdate() : Componentのupdate
	Actor::update();
	// 再計算
	mRectangle.x = mPosition.x - mTexture.width / 2.0f;
	mRectangle.y = mPosition.y - mTexture.height / 2.0f;
}

void PlayerActor::draw()
{
	Rectangle src = { 0, 0, (float)mTexture.width, (float)mTexture.height };
	Rectangle dst = { mPosition.x, mPosition.y, (float)mTexture.width, (float)mTexture.height };
	Vector2 origin = { mTexture.width / 2.0f, mTexture.height / 2.0f };
	DrawTexturePro(mTexture, src, dst, origin, 0.0f, WHITE);
}

const Camera2D& PlayerActor::getCamera() const
{
	return mCameraComp->getCamera();
}
