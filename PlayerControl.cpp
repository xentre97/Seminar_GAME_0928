#include "PlayerControl.h"
#include <raylib.h>
#include "PlayerActor.h"
#include <string>

PlayerControl::PlayerControl(PlayerActor* actor)
	: MoveComponent(actor)
	, mPlayer(actor)
	//, mMoveState(ms_idle)
	//, mActionState(as_idle)
	, mIsGuarding(false)
	, mDashSpeed(mMoveSpeed * 1.5f)
{
}

void PlayerControl::input()
{
	Vector2 pos = mOwner->getPosition();
	
	// 初期化
	mVelocityX = 0.0f;
	
	// 1.MoveStateの分岐
	switch (mPlayer->getMoveState())
	{
	case PlayerActor::ms_idle:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mPlayer->setMoveState(PlayerActor::ms_jump);
			mVelocityY = mJumpSpeed;
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				mPlayer->setMoveState(PlayerActor::ms_dash);
			}
			else {
				mPlayer->setMoveState(PlayerActor::ms_walk);
			}
		}
		break;
	}
	case PlayerActor::ms_walk:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mPlayer->setMoveState(PlayerActor::ms_jump);
			mVelocityY = mJumpSpeed;
		}
		else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
			mPlayer->setMoveState(PlayerActor::ms_idle);
		}
		else if (IsKeyDown(KEY_LEFT_SHIFT)) {
			mPlayer->setMoveState(PlayerActor::ms_dash);
		}
		break;
	}
	case PlayerActor::ms_dash:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mPlayer->setMoveState(PlayerActor::ms_jump);
			mVelocityY = mJumpSpeed;
		}
		else if (!IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
				mPlayer->setMoveState(PlayerActor::ms_walk);
			}
			else {
				mPlayer->setMoveState(PlayerActor::ms_idle);
			}
		}
		break;
	}
	case PlayerActor::ms_jump:
	{
		break;
	}
	}
	
	// 2.ActionStateの分岐
	switch (mPlayer->getActionState())
	{
	case PlayerActor::as_idle:
	{
		if (IsKeyPressed(KEY_W)) {
			mPlayer->setActionState(PlayerActor::as_attack);
			actionTimer = 0.0f;
		}
		else if (IsKeyDown(KEY_S)) {
			mPlayer->setActionState(PlayerActor::as_guard);
		}
		break;
	}
	case PlayerActor::as_attack:
	{
		break;
	}
	case PlayerActor::as_guard:
	{
		if (!IsKeyDown(KEY_S)) {
			mPlayer->setActionState(PlayerActor::as_idle);
		}
		break;
	}
	}

	// 自由落下
	if (mVelocityY > 0.0f && mPlayer->getMoveState() != PlayerActor::ms_jump) {
		mPlayer->setMoveState(PlayerActor::ms_jump);
	}

	if (IsKeyDown(KEY_D)) {
		mPlayer->setForward(1);
	}
	else if(IsKeyDown(KEY_A)) {
		mPlayer->setForward(-1);
	}
}

void PlayerControl::update()
{
	// 重力
	if (mVelocityY < 10) {
		mVelocityY += mGravity;
	}
	// 速度を設定
	Vector2 pos = mOwner->getPosition();

	// 1,MoveStateのupdate
	switch (mPlayer->getMoveState())
	{
	case PlayerActor::ms_idle:
	{
		break;
	}
	case PlayerActor::ms_walk:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mMoveSpeed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mMoveSpeed * GetFrameTime();
		}
		break;
	}
	case PlayerActor::ms_dash:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mDashSpeed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mDashSpeed * GetFrameTime();
		}
		break;
	}
	case PlayerActor::ms_jump:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mMoveSpeed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mMoveSpeed * GetFrameTime();
		}
		break;
	}
	}

	// 1,ActionStateのupdate
	switch (mPlayer->getActionState())
	{
	case PlayerActor::as_idle:
	{
		break;
	}
	case PlayerActor::as_attack:
	{
		actionTimer += 1.0f;
		break;
	}
	case PlayerActor::as_guard:
	{
		break;
	}
	}

	pos.x += mVelocityX;
	pos.y += mVelocityY;
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される	
	mOwner->setPosition(pos);

	if (mPlayer->getActionState() == PlayerActor::as_attack) {
		actionTimer += 1.0f;
		if (actionTimer > 60.0f) {
			mPlayer->setActionState(PlayerActor::as_idle);
		}
	}

	StateDraw();
}

// GamePlayで呼ぶ。床との衝突解消に使われる。
// 重力は常に働いているので地上にいるとき常に呼ばれる
void PlayerControl::fixFloorCol()
{
	// Y方向の速度を初期化
	mVelocityY = 0.0f;
	// 空中からの解消の場合はStateを変更する
	if (mPlayer->getMoveState()  == PlayerActor::ms_jump) {
		mPlayer->setMoveState(PlayerActor::ms_idle);
	}
}

void PlayerControl::StateDraw()
{
	// MoveState
	switch (mPlayer->getMoveState())
	{
	case PlayerActor::ms_idle: {
		DrawText("Move : Idle", 700, 50, 30, BLACK); break;
	}
	case PlayerActor::ms_walk: {

		DrawText("Move : Walk", 700, 50, 30, BLACK); break;
	}
	case PlayerActor::ms_dash: {
		DrawText("Move : Dash", 700, 50, 30, BLACK); break;
	}
	case PlayerActor::ms_jump: {
		DrawText("Move : Jump", 700, 50, 30, BLACK); break;
	}
	// ActionState
	}
	switch (mPlayer->getActionState())
	{
	case PlayerActor::as_idle: {
		DrawText("Action : idle", 700, 100, 30, BLACK); break;
	}
	case PlayerActor::as_attack: {
		DrawText("Action : attack", 700, 100, 30, BLACK); break;
	}
	case PlayerActor::as_guard: {
		DrawText("Action : guard", 700, 100, 30, BLACK); break;
	}
	}
}
