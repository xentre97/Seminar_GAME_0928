#include "PlayerControl.h"
#include <raylib.h>
#include <string>

#include "PlayerActor.h"

PlayerControl::PlayerControl(PlayerActor* owner)
	: MoveComponent(owner)
	, mPlayer(owner)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
	, mChargeTimer(0.0f)
	, mChargeTime(1.0f)
	, mLastPressTimeD(-1.0f)
	, mLastPressTimeA(-1.0f)
	, mDodgeTime(0.3f)
	, mDodgeTimer(0.0f)
	, mDodgeSpeed(500.0f)
{
}

void PlayerControl::input()
{
	Vector2 pos = mOwner->getPosition();
	
	// 横方向速度を初期化,回避中なら変更しない
	if (mPlayer->getActionState() != PlayerActor::as_dodge) {
		mVelocityX = 0.0f;
	}
	
	// 1.現在のMoveStateに応じた入力処理
	switch (mPlayer->getMoveState())
	{
	case PlayerActor::ms_idle:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mPlayer->changeState(PlayerActor::ms_jump);
			// 以下はchangeState内からやってもいい
			mVelocityY = mJumpSpeed;
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
			mPlayer->changeState(PlayerActor::ms_walk);
		}
		break;
	}
	case PlayerActor::ms_walk:
	{
		if (IsKeyPressed(KEY_SPACE)) {
			mPlayer->changeState(PlayerActor::ms_jump);
			mVelocityY = mJumpSpeed;
		}
		else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
			mPlayer->changeState(PlayerActor::ms_idle);
		}
		break;
	}
	case PlayerActor::ms_jump:
	{
		break;
	}
	}
	
	// 2.現在のActionStateに応じた入力処理
	switch (mPlayer->getActionState())
	{
	case PlayerActor::as_idle:
	{
		// 攻撃
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(PlayerActor::as_attack);
			mAttackTimer = 0.0f;
		}
		// ガード
		else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			mPlayer->changeState(PlayerActor::as_guard);
		}
		// 回避
		float now = GetTime(); // 現在時刻
		if (IsKeyPressed(KEY_D)) {
			if (now - mLastPressTimeD < 0.25f) {
				mPlayer->changeState(PlayerActor::as_dodge);
				mDodgeTimer = 0.0f;
				mVelocityX = mDodgeSpeed;
			}
			mLastPressTimeD = now;
		}
		if (IsKeyPressed(KEY_A)) {
			if (now - mLastPressTimeA < 0.25f) {
				mPlayer->changeState(PlayerActor::as_dodge);
				mDodgeTimer = 0.0f;
				mVelocityX = -mDodgeSpeed;
			}
			mLastPressTimeA = now;
		}

		break;
	}
	case PlayerActor::as_attack:
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mChargeTimer += GetFrameTime();
		}
		else {
			mChargeTimer = 0.0f;
		}
		break;
	}
	case PlayerActor::as_guard:
	{
		if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			mPlayer->changeState(PlayerActor::as_idle);
		}
		break;
	}
	case PlayerActor::as_charge:
	{
		mChargeTimer += GetFrameTime();
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (mChargeTimer > mChargeTime) {
				mPlayer->changeState(PlayerActor::as_attack);
			}
			else {
				mPlayer->changeState(PlayerActor::as_idle);
			}
			mChargeTimer = 0.0f;
		}
		break;
	}
	}

	// 自由落下
	if (mVelocityY > 0.0f && mPlayer->getMoveState() != PlayerActor::ms_jump) {
		mPlayer->changeState(PlayerActor::ms_jump);
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
	if (mVelocityY < 600.0f) { // 600.0fは加速の上限
		mVelocityY += mGravity;
	}
	// 位置をキャッシュ
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
			mVelocityX = mMoveSpeed;
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mMoveSpeed;
		}
		break;
	}
	case PlayerActor::ms_jump:
	{
		if (IsKeyDown(KEY_D)) {
			mVelocityX = mMoveSpeed;
		}
		else if (IsKeyDown(KEY_A)) {
			mVelocityX = -mMoveSpeed;
		}
		break;
	}
	}

	// 2.ActionStateのupdate
	switch (mPlayer->getActionState())
	{
	case PlayerActor::as_idle:
	{
		break;
	}
	case PlayerActor::as_attack:
	{
		/*if (mPlayer->getWeapon()->attackEnded()) {

		}*/
		mAttackTimer += GetFrameTime();
		if (mAttackTimer >= mAttackTime) {
			mAttackTimer = 0.0f;
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				mPlayer->changeState(PlayerActor::as_charge);
			}
			else {
				mPlayer->changeState(PlayerActor::as_idle);
			}
		}
		break;
	}
	case PlayerActor::as_guard:
	{
		break;
	}
	case PlayerActor::as_charge:
	{
		// charge中は速度を遅くする
		mVelocityX /= 2.0f;
		break;
	}
	case PlayerActor::as_dodge:
	{
		mDodgeTimer += GetFrameTime();
		mVelocityX /= 1.1f;
		// 回避中は移動入力を受け付けない
		if (mDodgeTimer >= mDodgeTime) {
			mPlayer->changeState(PlayerActor::as_idle);
			mDodgeTimer = 0.0f;
		}
		break;
	}
	}

	// 速度を設定
	pos.x += mVelocityX / GetFPS();
	pos.y += mVelocityY / GetFPS();
	// もし床にぶつかるとき,velocityYはGamePlayクラスの
	// updateCollision関数で0に戻される
	mOwner->setPosition(pos);
	mOwner->computeRectangle();

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
		mPlayer->changeState(PlayerActor::ms_idle);
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
	case PlayerActor::as_charge: {
		DrawText("Action : charge", 700, 100, 30, BLACK); break;
	}
	case PlayerActor::as_dodge: {
		DrawText("Action : dodge", 700, 100, 30, BLACK); break;
	}
	}
}
