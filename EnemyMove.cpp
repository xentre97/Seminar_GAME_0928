#include "EnemyMove.h"
#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(EnemyActor* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(32.0f)
	, mAttackTime(1.0f)
	, mAttackTimer(0.0f)
{
	mJumpSpeed = -600.0f;
	// �^�[�Q�b�g�w��(EnemyActor������w�肵�Ă�����)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	// ���R�����ւ̑J�ڔ�����ŏ��ɍs��
	if (mVelocityY > 0.0f && mEnemy->getEnemyState() != EnemyActor::E_jump) {
		mEnemy->changeState(EnemyActor::E_jump);
	}

	// �d��(�����ړ�)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	// �X�e�[�g�ɂ���ď������ς��
	switch (mEnemy->getEnemyState())
	{
	case EnemyActor::E_walk:
	{
		// �����˒��O�Ȃ�v���C���[�̕��ֈړ�����
		if (abs(ownerPos.x - targetPos.x) > mAttackRange ||
			abs(ownerPos.y - targetPos.y) > mAttackRange) {
			// ���g�̈ʒu�ɑ΂���v���C���[�̈ʒu�̕������擾(���g���Ȃ�-1�A���Ȃ�1�ɐi��)
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
			//�����ړ��̑��x��ݒ�
			mVelocityX = mOwner->getForward() * mMoveSpeed;
		}
		// �˒����Ȃ�U����Ԃ�
		else {
			mEnemy->changeState(EnemyActor::E_attack);
			mVelocityX = 0.0f;	// attack���ɑ��x��0�ɂ��Ă݂�
		}
		break;
	}
	case EnemyActor::E_jump:
	{
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);
		//�����ړ��̑��x��ݒ�
		mVelocityX = mOwner->getForward() * mMoveSpeed;
		break;
	}
	case EnemyActor::E_attack:
	{
		// ��莞�Ԃ������� || �˒��O�ɂȂ�����
		mAttackTimer += GetFrameTime();
		if (mAttackTime < mAttackTimer ||
			abs(ownerPos.x - targetPos.x) > mAttackRange ||
			abs(ownerPos.y - targetPos.y) > mAttackRange) {
			mEnemy->changeState(EnemyActor::E_walk);
			// ���g�̈ʒu�ɑ΂���v���C���[�̈ʒu�̕������擾(���g���Ȃ�-1�A���Ȃ�1�ɐi��)
			int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
			mOwner->setForward(xDirection);
			mAttackTimer = 0.0f;
		}
		break;
	}
	}

	//���x���ʒu�ɔ��f
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();
	mOwner->setPosition(ownerPos);
	// Enemy�̈ړ������S�ɏI��������̈ʒu����ɒ����`�悷�邽�߂�Rectangle�ʒu����
	mOwner->computeRectangle();

	// �ȉ��f�o�b�O�p
	switch (mEnemy->getEnemyState())
	{
	case EnemyActor::E_walk: {
		DrawText("Enemy : walk", 700, 150, 30, BLACK); break;
	}
	case EnemyActor::E_jump: {

		DrawText("Enemy : jump", 700, 150, 30, BLACK); break;
	}
	case EnemyActor::E_attack: {
		DrawText("Enemy : attack", 700, 150, 30, BLACK); break;
	}
	}
}

void EnemyMove::fixFloorCol()
{
	// Y�����̑��x��������
	mVelocityY = 0.0f;
	// �󒆂���̉����̏ꍇ��State��ύX����
	if (mEnemy->getEnemyState() == EnemyActor::E_jump) {
		mEnemy->changeState(EnemyActor::E_walk);
	}
}
