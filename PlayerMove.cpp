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
	// ���R�����̑J�ڂ͂����ōs��
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
	// �d��
	if (mVelocityY < 600.0f) { // 600.0f�͉����̏��
		mVelocityY += mGravity;
	}
	// �ʒu���L���b�V��
	Vector2 pos = mOwner->getPosition();

	// ���x��ݒ�
	pos.x += mVelocityX * mPlayer->getForward() / GetFPS();
	pos.y += mVelocityY / GetFPS();
	// �������ɂԂ���Ƃ�,velocityY��GamePlay�N���X��
	// updateCollision�֐���0�ɖ߂����
	mOwner->setPosition(pos);
	mOwner->computeRectangle();

	// ���������x��������,���,����U�����Ȃ�ύX���Ȃ�
	if (mPlayer->getPlayerState()->getType() != PlayerState::Type::Dodge &&
		mPlayer->getPlayerState()->getType() != PlayerState::Type::DodgeAttack) {
		mVelocityX = 0.0f;
	}

	StateDraw();
}

// GamePlay�ŌĂԁB���Ƃ̏Փˉ����Ɏg����B
// �d�͂͏�ɓ����Ă���̂Œn��ɂ���Ƃ���ɌĂ΂��
void PlayerMove::fixFloorCol()
{
	// Y�����̑��x��������
	mVelocityY = 0.0f;
	// �󒆂���̉����̏ꍇ��State��ύX����
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
