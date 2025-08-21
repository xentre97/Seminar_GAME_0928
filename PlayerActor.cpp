#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerControl.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence)
{
	mTexture = LoadTexture("testPlayerIdle.png");
	mPosition = Vector2{ 0.0f, 0.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	mMoveTextures[PlayerControl::ms_idle] = LoadTexture("testPlayerIdle.png");
	mMoveTextures[PlayerControl::ms_jump] = LoadTexture("testPlayerJump.png");

	mCameraComp = new CameraComponent(this);
	mPlayerControl = new PlayerControl(this);
}

void PlayerActor::input()
{
	// Šî’ê‚Ìinput() : Component‚Ìinput
	Actor::input();
}

void PlayerActor::update()
{
	// Šî’ê‚Ìupdate() : Component‚Ìupdate
	Actor::update();
	// ‹éŒ`ÄŒvŽZ
	computeRectangle();
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

PlayerControl& PlayerActor::getPlayerControl()
{
	return *mPlayerControl;
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mTexture.width / 2.0f;
	mRectangle.y = mPosition.y - mTexture.height / 2.0f;
}

void PlayerActor::setMoveTexture(PlayerControl::MoveState s)
{
	mTexture = mMoveTextures[s];
}
