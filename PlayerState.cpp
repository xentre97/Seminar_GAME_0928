#include "PlayerState.h"
#include <raylib.h>
#include "PlayerActor.h"
#include "Sequence.h"

#include "PlayerMove.h"
#include "WeaponComponent.h"
#include "AnimSpriteComponent.h"


// �A�j���[�V�����̍Đ����s��
void PlayerState::enter()
{
	mPlayer->getAnimSpriteComp()->play(&mAnim);
}

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
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerIdle.png")};
	mAnim.frames = frames;
	mAnim.loop = true;
}

void Idle::input()
{
	// �W�����v
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	// �ړ�
	else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		// ���������͂Ȃ�����
		if (IsKeyDown(KEY_A) && GetTime() - mLastPressedTimeA < mDoubleTapWindow ||
			IsKeyDown(KEY_D) && GetTime() - mLastPressedTimeD < mDoubleTapWindow) {
			mPlayer->changeState(Type::Dodge);
		}
		// �����łȂ��Ȃ������
		else {
			mPlayer->changeState(Type::Walk);
		}
	}
	// �ʏ�U��
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
	// �K�[�h
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}

	// ��ԑJ�ڔ����,�Ō��A,D�����������Ԃ��L�^
	if (IsKeyPressed(KEY_A)) mLastPressedTimeA = GetTime();
	if (IsKeyPressed(KEY_D)) mLastPressedTimeD = GetTime();
}

void Idle::enter()
{
	PlayerState::enter();
	mPlayer->getPlayerMove()->setVelocityX(0.0f);
}

Walk::Walk(PlayerActor* player)
	: PlayerState(player, Type::Walk)
	, mWalkSpeed(180.0f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerWalk.png") };
	mAnim.frames = frames;
	mAnim.loop = true;
}

void Walk::input()
{
	// �W�����v
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->changeState(Type::Jump);
	}
	// Idle
	else if (!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
		mPlayer->changeState(Type::Idle);
	}
	// �K�[�h
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		mPlayer->changeState(Type::Guard);
	}
	// �ʏ�U��
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::NormalAttack);
	}
}

void Walk::update()
{
	// ���������x��ݒ肵������
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
		mPlayer->getPlayerMove()->setVelocityX(mWalkSpeed);
	}
}

Jump::Jump(PlayerActor* player)
	: PlayerState(player, Type::Jump)
	, mHorizontalSpeed(180.0f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerJump.png") };
	mAnim.frames = frames;
	mAnim.loop = true;
}

void Jump::enter()
{
	PlayerState::enter();
	// �X�y�[�X��������Ă����Ȃ�W�����v
	// �����łȂ��Ȃ玩�R����
	if (IsKeyPressed(KEY_SPACE)) {
		mPlayer->getPlayerMove()->jump();
	}
}

void Jump::update()
{
	// ���������x��ݒ肵������
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
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerDodge.png") };
	mAnim.frames = frames;
	mAnim.loop = true;
}

void Dodge::enter()
{
	PlayerState::enter();
	mDodgeSpeed = 500.0f;
	mPlayer->getPlayerMove()->setVelocityX(mDodgeSpeed);
}

void Dodge::update()
{
	mDodgeTimer += GetFrameTime();
	mDodgeSpeed /= 1.1f;	// ���x����
	mPlayer->getPlayerMove()->setVelocityX(mDodgeSpeed);
	// idle��
	if (mDodgeTimer >= mDodgeTime) {
		mPlayer->changeState(Type::Idle);
		mDodgeTimer = 0.0f;
	}
	// DodgeAttack��
	else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		mPlayer->changeState(Type::DodgeAttack);
	}
}

Charge::Charge(PlayerActor* player)
	: PlayerState(player, Type::Charge)
	, mChargeTimer(0.0f)
	, mChargeTime(1.0f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerCharge.png") };
	mAnim.frames = frames;
	mAnim.loop = true;
}

void Charge::input()
{
	mChargeTimer += GetFrameTime();
	// �{�^���𗣂�����
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		// ���ߍU����
		if (mChargeTimer > mChargeTime) {
			mPlayer->changeState(Type::ChargeAttack);
		}
		// Idle��
		else {
			mPlayer->changeState(Type::Idle);
		}
		mChargeTimer = 0.0f;
	}
}

void Charge::update()
{
	// ������ƈړ���x�����Ă݂�
	float velX = mPlayer->getPlayerMove()->getVelocityX();
	mPlayer->getPlayerMove()->setVelocityX(velX / 2);
}

NormalAttack::NormalAttack(PlayerActor* player)
	: PlayerState(player, Type::NormalAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerNormalAttack.png") };
	mAnim.frames = frames;
	mAnim.loop = false;
}

void NormalAttack::update()
{
	mAttackTimer += GetFrameTime();
	// �U�����Ԃ��߂�����
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// ���߂�
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		// Idle��
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void NormalAttack::enter()
{
	PlayerState::enter();
	mPlayer->getWeapon()->startAttack(AttackType::Normal);
}

void NormalAttack::exit()
{
	// �Ȃ�������
	mPlayer->getWeapon()->endAttack();
}

DodgeAttack::DodgeAttack(PlayerActor* player)
	: PlayerState(player, Type::DodgeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
	, mHorizontalSpeed(0.0f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerDodgeAttack.png") };
	mAnim.frames = frames;
	mAnim.loop = false;
}

void DodgeAttack::update()
{
	mHorizontalSpeed /= 1.1f;	// ���x����
	mPlayer->getPlayerMove()->setVelocityX(mHorizontalSpeed);
	mAttackTimer += GetFrameTime();
	// �U�����Ԃ��߂�����
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// ���߂�
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			mPlayer->changeState(Type::Charge);
		}
		// Idle��
		else {
			mPlayer->changeState(Type::Idle);
		}
	}
}

void DodgeAttack::enter()
{
	PlayerState::enter();
	mPlayer->getWeapon()->startAttack(AttackType::Dodge);
	mHorizontalSpeed = mPlayer->getPlayerMove()->getVelocityX();
}

void DodgeAttack::exit()
{
	// �Ȃ�������
	mPlayer->getWeapon()->endAttack();
}

ChargeAttack::ChargeAttack(PlayerActor* player)
	: PlayerState(player, Type::ChargeAttack)
	, mAttackTimer(0.0f)
	, mAttackTime(0.5f)
{
	std::vector<Texture2D*> frames = { mPlayer->getSequence()->getTexture("testPlayerChargeAttack.png") };
	mAnim.frames = frames;
	mAnim.loop = false;
}

void ChargeAttack::update()
{
	mAttackTimer += GetFrameTime();
	// �U�����Ԃ��߂�����
	if (mAttackTimer >= mAttackTime) {
		mAttackTimer = 0.0f;
		// Idle��
		mPlayer->changeState(Type::Idle);
	}
}

void ChargeAttack::enter()
{
	PlayerState::enter();
	mPlayer->getWeapon()->startAttack(AttackType::Charge);
}

void ChargeAttack::exit()
{
	// �Ȃ�������
	mPlayer->getWeapon()->endAttack();
}