#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerMove.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence)
{
	mTexture = LoadTexture("testPlayer.png");
	mPosition = Vector2{ 0.0f, 0.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	mCameraComp = new CameraComponent(this);
	mPlayerMove = new PlayerMove(this);
}

void PlayerActor::input()
{
	// 基底のinput() : Componentのinput
	Actor::input();
	
	//// AD : 横に移動
	//if (IsKeyDown(KEY_A)) {
	//	//mPosition.x -= 180 * GetFrameTime();
	//	mPosition.x -= 3;
	//}
	//if (IsKeyDown(KEY_D)) {
	//	//mPosition.x += 180 * GetFrameTime();
	//	mPosition.x += 3;
	//}
	//// Space : ジャンプ
	//if (IsKeyDown(KEY_SPACE) && !mJumping  )
	//{
	//	// ジャンプ
	//}



}

void PlayerActor::update()
{
	// 基底のupdate() : Componentのupdate
	Actor::update();
	// 再計算
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

PlayerMove& PlayerActor::getPlayerMove()
{
	return *mPlayerMove;
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mTexture.width / 2.0f;
	mRectangle.y = mPosition.y - mTexture.height / 2.0f;
}
