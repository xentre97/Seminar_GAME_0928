#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerControl.h"
#include "AnimSpriteComponent.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence)
	, mMoveState(ms_idle)
	, mActionState(as_idle)
{
	mTexture = LoadTexture("testPlayerIdle.png");
	mPosition = Vector2{ 0.0f, 0.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	mAnimsc = new AnimSpriteComponent(this);
	std::vector<Texture2D> texs = {
		LoadTexture("testPlayerIdle.png"),
		LoadTexture("testPlayerJump.png"),
		LoadTexture("testPlayerWalk.png"),
		LoadTexture("testPlayerDash.png")
	};
	// SetTetureはAnimSpriteComponentの関数で一つ一つ行う
	mAnimsc->setAnimTextures(texs);

	mCameraComp = new CameraComponent(this);
	mPlayerControl = new PlayerControl(this);
}

void PlayerActor::input()
{
	// 基底のinput() : Componentのinput
	Actor::input();
}

void PlayerActor::update()
{
	// 基底のupdate() : Componentのupdate
	Actor::update();
	// 矩形再計算
	computeRectangle();

	// とりあえずの実装(updateする度にplayする必要はない)
	// ステート変更時に呼び出す関数,onChangeStateみたいなのを作ればいい
	// 複雑な処理が必要ならstateのonEnter,onExitのような関数を作る
	switch (mMoveState)
	{
	case ms_idle:
	{
		mAnimsc->play(0, 0, true); break;
	}
	case ms_jump:
	{
		mAnimsc->play(1, 1, true); break;
	}
	case ms_walk:
	{
		mAnimsc->play(2, 2, true); break;
	}
	case ms_dash:
	{
		mAnimsc->play(3, 3, true); break;
	}
	}
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
