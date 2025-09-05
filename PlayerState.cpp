#include "PlayerState.h"
#include <raylib.h>
#include "PlayerActor.h"

#include "PlayerMove.h"
#include "WeaponComponent.h"
#include "AnimSpriteComponent.h"


PlayerState::PlayerState(PlayerActor* player, Type type)
	: mPlayer(player)
	, mType(type)
{
}


Idle::Idle(PlayerActor* player)
	: PlayerState(player, Type::Idle)
	, mLastPressedTimeA(0.0)
	, mLastPressedTimeD(0.0)
	, mDoubleTapWindow(0.2)
{
}

void Idle::input()
{
	// ƒWƒƒƒ“ƒv
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	// ˆÚ“®
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		// ‚à‚µ“ñ‰ñ“ü—Í‚È‚ç‰ñ”ð‚Ö
		if (IsKeyDown(KEY_A) && GetTime() - mLastPressedTimeA < mDoubleTapWindow ||
			IsKeyDown(KEY_D) && GetTime() - mLastPressedTimeD < mDoubleTapWindow) {
			mPlayer->changeState(Type::Dodge);
		}
		// ‚»‚¤‚Å‚È‚¢‚È‚ç•à‚«‚Ö
		else {
			mPlayer->changeState(Type::Walk);
		}
	}
	// ’ÊíUŒ‚
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
	// ƒK[ƒh
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}

	// ó‘Ô‘JˆÚ”»’èŒã,ÅŒã‚ÉA,D‚ð‰Ÿ‚µ‚½ŽžŠÔ‚ð‹L˜^
	if (IsKeyPressed(KEY_A)) mLastPressedTimeA = GetTime();
	if (IsKeyPressed(KEY_D)) mLastPressedTimeD = GetTime();
}

void Idle::enter()
{
	mPlayer->getPlayerMove()->setVelocityX(0.0f);
	mPlayer->getAnimSpriteComp()->play("Idle");
}

Walk::Walk(PlayerActor* player)
	: PlayerState(player, Type::Walk)
	, mWalkSpeed(180.0f)
{
}

void Walk::input()
{
	// ƒWƒƒƒ“ƒv
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	// Idle
	else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
		mPlayer->changeState(Type::Idle);
	}
	// ƒK[ƒh
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}
	// ’ÊíUŒ‚
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
}

void Walk::update()
{
	// ‰¡•ûŒü‘¬“x‚ðÝ’è‚µ‘±‚¯‚é
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		mPlayer->getPlayerMove()->setVelocityX(mWalkSpeed);
	}
}

void Walk::enter()
{
	mPlayer->getAnimSpriteComp()->play("Walk");
}

Jump::Jump(PlayerActor* player)
	: PlayerState(player, Type::Jump)
	, mHorizontalSpeed(180.0f)
{
}

void Jump::enter()
{
	// ƒXƒy[ƒX‚ª‰Ÿ‚³‚ê‚Ä‚¢‚½‚È‚çƒWƒƒƒ“ƒv
	// ‚»‚¤‚Å‚È‚¢‚È‚çŽ©—R—Ž‰º
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->getPlayerMove()->jump();
	}
	mPlayer->getAnimSpriteComp()->play("Jump");
}

void Jump::update()
{
	// ‰¡•ûŒü‘¬“x‚ðÝ’è‚µ‘±‚¯‚é
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		mPlayer->getPlayerMove()->setVelocityX(mHorizontalSpeed);
	}
}

Guard::Guard(PlayerActor* player)
	: PlayerState(player, Type::Guard)
{
}


Dodge::Dodge(PlayerActor* player)
	: PlayerState(player, Type::Dodge)
	, mDodgeTime(0.3f)
	, mDodgeTimer(0.0f)
	, mDodgeSpeed(500.0f)
{
}

void Dodge::enter()
{
	mDodgeSpeed = 500.0f;
	mPlayer->getPlayerMove()->setVelocityX(mDodgeSpeed);
	mPlayer->getAnimSpriteComp()->play("Dodge");
}

void Dodge::update()
{
	mDodgeTimer += GetFrameTime();
	mDodgeSpeed /= 1.1f;	// ‘¬“xŒ¸Š
	// idle‚Ö
	if (mDodgeTimer >= mDodgeTime) {
		mPlayer->changeState(Type::Idle);
		mDodgeTimer = 0.0f;
	}
	// DodgeAttack‚Ö
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::DodgeAttack);
	}
}

Charge::Charge(PlayerActor* player)
	: PlayerState(player, Type::Charge)
	, mChargeTimer(0.0f)
	, mChargeTime(1.0f)
{
}

void Charge::input()
{
	mChargeTimer += GetFrameTime();
	// ƒ{ƒ^ƒ“‚ð—£‚µ‚½Žž
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		// ‚½‚ßUŒ‚‚Ö
		if (mChargeTimer > mChargeTime) {
			mPlayer->changeState(Type::ChargeAttack);
		}
		// Idle‚Ö
		else {
			mPlayer->changeState(Type::Idle);
		}
		mChargeTimer = 0.0f;
	}
}

void Charge::update()
{
	// ‚¿‚å‚Á‚ÆˆÚ“®‚ð’x‚­‚µ‚Ä‚Ý‚é
	float velX = mPlayer->getPlayerMove()->getVelocityX();
	mPlayer->getPlayerMove()->setVelocityX(velX / 2);
}

void Charge::enter()
{
	mPlayer->getAnimSpriteComp()->play("Charge");
}

NormalAttack::NormalAttack(PlayerActor* player)
	: PlayerState(player, Type::NormalAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
}

void NormalAttack::update()
{
	mAttackTimer += GetFrameTime();
	// UŒ‚ŽžŠÔ‚ð‰ß‚¬‚½‚ç
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// ‚½‚ß‚Ö
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		// Idle‚Ö
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void NormalAttack::enter()
{
	mPlayer->getAnimSpriteComp()->play("NormalAttack");
	mPlayer->getWeapon()->startAttack();
}


DodgeAttack::DodgeAttack(PlayerActor* player)
	: PlayerState(player, Type::DodgeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
}

void DodgeAttack::update()
{
	mAttackTimer += GetFrameTime();
	// UŒ‚ŽžŠÔ‚ð‰ß‚¬‚½‚ç
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// ‚½‚ß‚Ö
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		// Idle‚Ö
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void DodgeAttack::enter()
{
	mPlayer->getAnimSpriteComp()->play("DodgeAttack");
	mPlayer->getWeapon()->startAttack();
}

ChargeAttack::ChargeAttack(PlayerActor* player)
	: PlayerState(player, Type::ChargeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
}

void ChargeAttack::update()
{
	mAttackTimer += GetFrameTime();
	// UŒ‚ŽžŠÔ‚ð‰ß‚¬‚½‚ç
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// Idle‚Ö
		mPlayer->changeState(Type::Idle);
	}
}

void ChargeAttack::enter()
{
	mPlayer->getAnimSpriteComp()->play("ChargeAttack");
	mPlayer->getWeapon()->startAttack();
}
