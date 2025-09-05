#include "PlayerMove.h"
#include <raylib.h>
#include <string>

#include "PlayerActor.h"

PlayerMove::PlayerMove(PlayerActor* owner)
	: MoveComponent(owner)
	, mPlayer(owner)
{
}

void PlayerMove::input()
{
	// 自由落下の遷移はここで行う
	if (mVelocityY > 0.0f && mPlayer->getPlayerState()->getType() != PlayerState::Type::Jump) {
		mPlayer->changeState(PlayerState::Type::Jump);
	}

	if (IsKeyDown(KEY_D)) {
		mPlayer->setForward(1);
	}
	else if(IsKeyDown(KEY_A)) {
		mPlayer->setForward(-1);
	}
}

void PlayerMove::update()
{
	// 重力
	if (mVelocityY < 600.0f) { // 600.0fは加速の上限
		mVelocityY += mGravity;
	}
	// 位置をキャッシュ
	Vector2 pos = mOwner->getPosition();

	// 速度を設定
	pos.x += mVelocityX * mPlayer->getForward() / GetFPS();
	pos.y += mVelocityY / GetFPS();
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される
	mOwner->setPosition(pos);
	mOwner->computeRectangle();

	// 横方向速度を初期化,回避中,回避攻撃中なら変更しない
	if (mPlayer->getPlayerState()->getType() != PlayerState::Type::Dodge &&
		mPlayer->getPlayerState()->getType() != PlayerState::Type::DodgeAttack) {
		mVelocityX = 0.0f;
	}

	StateDraw();
}

// GamePlayで呼ぶ。床との衝突解消に使われる。
// 重力は常に働いているので地上にいるとき常に呼ばれる
void PlayerMove::fixFloorCol()
{
	// Y方向の速度を初期化
	mVelocityY = 0.0f;
	// 空中からの解消の場合はStateを変更する
	if (mPlayer->getPlayerState()->getType() == PlayerState::Type::Jump) {
		mPlayer->changeState(PlayerState::Type::Idle);
	}
}

void PlayerMove::jump()
{
	mVelocityY = mJumpSpeed;
}

void PlayerMove::StateDraw()
{
	// MoveState
	switch (mPlayer->getPlayerState()->getType())
	{
	case PlayerState::Type::Idle: {
		DrawText("Player : Idle", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::Walk: {

		DrawText("Player : Walk", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::Jump: {
		DrawText("Player : Jump", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::Dodge: {
		DrawText("Player : Dodge", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::Guard: {
		DrawText("Player : Guard", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::Charge: {
		DrawText("Player : Charge", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::NormalAttack: {
		DrawText("Player : nAttack", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::DodgeAttack: {
		DrawText("Player : dAttack", 700, 50, 30, BLACK); break;
	}
	case PlayerState::Type::ChargeAttack: {
		DrawText("Player : cAttack", 700, 50, 30, BLACK); break;
	}
	}
}
