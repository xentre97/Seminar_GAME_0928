#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "PlayerMove.h"
#include "AnimSpriteComponent.h"
#include "SwordComponent.h"
#include "ArrowComponent.h"
#include "HpComponent.h"

#include "GamePlay.h"

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
	
	// ��������͌�
	mWeaponComp = new SwordComponent(this);
	//mPlayerStates[PlayerState::Type::NormalAttack]->setAnimation();
	//mPlayerStates[PlayerState::Type::DodgeAttack]->setAnimation();
	//mPlayerStates[PlayerState::Type::ChargeAttack]->setAnimation();

	// ���݂̏�Ԃ�ݒ�
	mPlayerState = mPlayerStates[PlayerState::Type::Idle];
}

PlayerActor::~PlayerActor()
{
	for (auto state : mPlayerStates) {
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
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}

void PlayerActor::changeState(PlayerState::Type type)
{
	mPlayerState->exit();
	mPlayerState = mPlayerStates[type];
	mPlayerState->enter();
}
