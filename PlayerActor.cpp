#include "PlayerActor.h"
#include "EnemyActor.h"
#include "StageObject.h"
#include "PlayerMove.h"
#include "AnimSpriteComponent.h"
#include "HpComponent.h"
#include "AttackComponent.h"

#include "GamePlay.h"
#include "Stage.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence, Actor::Eplayer)
{
	Texture2D* tex  = mSequence->getTexture("testPlayerIdle.png");
	mPosition = Vector2{ 100.0f, 200.0f };
	mRectangle = {
		mPosition.x - tex->width / 2.0f,
		mPosition.y - tex->height / 2.0f,
		(float)tex->width,
		(float)tex->height
	};
	mAnimsc = new AnimSpriteComponent(this);

	mPlayerMove = new PlayerMove(this);
	mHpComp = new HpComponent(this, 200.0f);

	// ��Ԃ��}�b�v�ɓo�^ ��Ԃ͈�C��new,delete����
	// ���R:��Ԑ؂�ւ��̓x��new,delete�̓t���O�����e�[�V�������C�ɂȂ邩��W
	mPlayerStates[PlayerState::Type::Idle] = new Idle(this);
	mPlayerStates[PlayerState::Type::Walk] = new Walk(this);
	mPlayerStates[PlayerState::Type::Jump] = new Jump(this);
	mPlayerStates[PlayerState::Type::Dodge] = new Dodge(this);
	mPlayerStates[PlayerState::Type::Guard] = new Guard(this);
	mPlayerStates[PlayerState::Type::Charge] = new Charge(this);
	mPlayerStates[PlayerState::Type::NormalAttack] = new NormalAttack(this);
	mPlayerStates[PlayerState::Type::DodgeAttack] = new DodgeAttack(this);
	mPlayerStates[PlayerState::Type::ChargeAttack] = new ChargeAttack(this);
	
	mAttackComp = new AttackComponent(this);

	// ���݂̏�Ԃ�ݒ�
	mPlayerState = mPlayerStates[PlayerState::Type::Idle];
}

PlayerActor::~PlayerActor()
{
	for (auto& state : mPlayerStates) {
		delete state.second;
	}
}

void PlayerActor::input()
{
	// ����input() : Component��input
	Actor::input();
	mPlayerState->input();
}

void PlayerActor::update()
{
	// ����update() : Component��update
	Actor::update();
	mPlayerState->update();

	fixCollision();
}

void PlayerActor::computeRectangle()
{
	//mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	//mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
	//mRectangle.width = mAnimsc->getTexWidth();
	//mRectangle.height = mAnimsc->getTexHeight();

	//�n�[�h�R�[�f�B���O���Ă݂��i�}�`����͂��ċ��炭���̒��x�Ƃ����ڎ��ɂ��n�C�p�[�p�����[�^�j
	mRectangle.width = 21.0f;
	mRectangle.height = 35.0f;
	mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
	mRectangle.y = mPosition.y - mRectangle.height / 2.0f;


}

void PlayerActor::changeState(PlayerState::Type type)
{
	mPlayerState->exit();
	mPlayerState = mPlayerStates[type];
	mPlayerState->enter();
}

void PlayerActor::fixCollision()
{
	// �X�e�[�W�Ƃ̓����蔻��
	for (auto& stageRec : static_cast<GamePlay*>(mSequence)->getStage()->getStageRecs()) {
		stageCollision(stageRec);
	}
	// �j��\�I�u�W�F�N�g�Ƃ̓����蔻��
	for (auto& obj: static_cast<GamePlay*>(mSequence)->getStageObjs()) {
		
		stageCollision(obj->getRectangle());
	}

	// ��ʍ��[�Ƃ̓����蔻��
	Vector2 screenPos = GetWorldToScreen2D(mPosition, static_cast<GamePlay*>(mSequence)->getCamera());
	if (screenPos.x < 0.0f) {
		mPosition.x -= screenPos.x;
		computeRectangle();
	}
}

void PlayerActor::stageCollision(const Rectangle &stageRec)
{
	// �����蔻��
	if (CheckCollisionRecs(mRectangle, stageRec)) {
		// �Փ˗̈���擾
		Rectangle colRec = GetCollisionRec(mRectangle, stageRec);
		// �c�����̏Փ�
		if (colRec.width >= colRec.height) {
			// �ォ��Փ�
			if (mRectangle.y < colRec.y) {
				mPosition.y -= colRec.height;
				mPlayerMove->fixFloorCol();
			}
			// ������Փ�
			else {
				mPosition.y += colRec.height;
				mPlayerMove->fixCeilingCol();
			}
		}
		// �������̏Փ�
		else {
			// ������Փ�
			if (mRectangle.x < colRec.x) {
				mPosition.x -= colRec.width;
			}
			// �E����Փ�
			else {
				mPosition.x += colRec.width;
			}
		}
		// ��`���Čv�Z
		computeRectangle();
	}
}