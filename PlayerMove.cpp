#include "PlayerMove.h"
#include <raylib.h>
#include "Actor.h"

PlayerMove::PlayerMove(Actor* actor)
	: MoveComponent(actor)
{
}

void PlayerMove::input()
{
	Vector2 pos = mOwner->getPosition();
	
	// 初期設定
	mVelocityX = 0.0f;
	// 重力がそのまま働いたとき
	if (mVelocityY > 0.0f && !mIsJumping) {
		mIsJumping = true;
	}

	// AD : 横に移動
	if (IsKeyDown(KEY_A)) {
		mVelocityX -= mMoveSpeed * GetFrameTime();
	}
	if (IsKeyDown(KEY_D)) {
		mVelocityX += mMoveSpeed * GetFrameTime();
	}
	// Space : ジャンプ
	if (IsKeyPressed(KEY_SPACE) && !mIsJumping)
	{
		mIsJumping = true;
		mVelocityY = mJumpSpeed;
	}
}

void PlayerMove::update()
{
	// 重力
	MoveComponent::update();
	// 速度を設定
	Vector2 pos = mOwner->getPosition();
	pos.x += mVelocityX;
	pos.y += mVelocityY;
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される	

	mOwner->setPosition(pos);
}
