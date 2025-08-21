#include "PlayerControl.h"
#include <raylib.h>
#include "PlayerActor.h"
#include <string>

PlayerControl::PlayerControl(PlayerActor* actor)
	: MoveComponent(actor)
	, mPlayer(actor)
	, mMoveState(ms_idle)
	, mActionState(as_idle)
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
	switch (mMoveState)
	{
	case ms_idle:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mMoveState = ms_jump;
			mVelocityY = mJumpSpeed;
			mPlayer->setMoveTexture(ms_jump);
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				mMoveState = ms_dash;
			}
			else {
				mMoveState = ms_walk;
			}
		}
		break;
	}
	case ms_walk:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mMoveState = ms_jump;
			mVelocityY = mJumpSpeed;
			mPlayer->setMoveTexture(ms_jump);
		}
		else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
			mMoveState = ms_idle;
		}
		else if (IsKeyDown(KEY_LEFT_SHIFT)) {
			mMoveState = ms_dash;
		}
		break;
	}
	case ms_dash:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mMoveState = ms_jump;
			mVelocityY = mJumpSpeed;
			mPlayer->setMoveTexture(ms_jump);
		}
		else if (!IsKeyDown(KEY_LEFT_SHIFT)) {
			if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
				mMoveState = ms_walk;
			}
			else {
				mMoveState = ms_idle;
			}
		}
		break;
	}
	case ms_jump:
	{
		break;
	}
	}
	
	// 2.ActionStateの分岐
	switch (mActionState)
	{
	case as_idle:
	{
		if (IsKeyPressed(KEY_W)) {
			mActionState = as_attack;
			actionTimer = 0.0f;
		}
		else if (IsKeyDown(KEY_S)) {
			mActionState = as_guard;
		}
		break;
	}
	case as_attack:
	{
		break;
	}
	case as_guard:
	{
		if (!IsKeyDown(KEY_S)) {
			mActionState = as_idle;
		}
		break;
	}
	}

	// 自由落下
	if (mVelocityY > 0.0f && mMoveState != ms_jump) {
		mMoveState = ms_jump;
		mPlayer->setMoveTexture(ms_jump);
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
	switch (mMoveState)
	{
	case ms_idle:
	{
		break;
	}
	case ms_walk:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mMoveSpeed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mMoveSpeed * GetFrameTime();
		}
		break;
	}
	case ms_dash:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mDashSpeed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mDashSpeed * GetFrameTime();
		}
		break;
	}
	case ms_jump:
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
	switch (mActionState)
	{
	case as_idle:
	{
		break;
	}
	case as_attack:
	{
		actionTimer += 1.0f;
		break;
	}
	case as_guard:
	{
		break;
	}
	}

	pos.x += mVelocityX;
	pos.y += mVelocityY;
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される	
	mOwner->setPosition(pos);

	if (mActionState == as_attack) {
		actionTimer += 1.0f;
		if (actionTimer > 60.0f) {
			mActionState = as_idle;
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
	mPlayer->setMoveTexture(ms_idle);
	// 空中からの解消の場合はStateを変更する
	if (mMoveState == ms_jump) {
		mMoveState = ms_idle;
	}
}

void PlayerControl::StateDraw()
{
	// MoveState
	switch (mMoveState)
	{
	case ms_idle: {
		DrawText("Move : Idle", 700, 50, 30, BLACK); break;
	}
	case ms_walk: {

		DrawText("Move : Walk", 700, 50, 30, BLACK); break;
	}
	case ms_dash: {
		DrawText("Move : Dash", 700, 50, 30, BLACK); break;
	}
	case ms_jump: {
		DrawText("Move : Jump", 700, 50, 30, BLACK); break;
	}
	// ActionState
	}
	switch (mActionState)
	{
	case as_idle: {
		DrawText("Action : idle", 700, 100, 30, BLACK); break;
	}
	case as_attack: {
		DrawText("Action : attack", 700, 100, 30, BLACK); break;
	}
	case as_guard: {
		DrawText("Action : guard", 700, 100, 30, BLACK); break;
	}
	}
}
