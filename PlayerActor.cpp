#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerControl.h"
#include "AnimSpriteComponent.h"
#include "SwordComponent.h"
#include "ArrowComponent.h"

#include "GamePlay.h"

PlayerActor::PlayerActor(Sequence* sequence, Type type)
	: Actor(sequence, type)
	, mMoveState(ms_idle)
	, mActionState(as_idle)
	, mSwordComp(nullptr)
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

	// アニメーションを追加
	std::vector<Texture2D*> idleTexs = { mSequence->getTexture("testPlayerIdle.png") };
	std::vector<Texture2D*> walkTexs = { mSequence->getTexture("testPlayerWalk.png") };
	std::vector<Texture2D*> dashTexs = { mSequence->getTexture("testPlayerDash.png") };
	std::vector<Texture2D*> jumpTexs = { mSequence->getTexture("testPlayerJump.png") };
	// 第一引数は、playを呼ぶときに使う
	mAnimsc->addAnimation("Idle", idleTexs);
	mAnimsc->addAnimation("Walk", walkTexs);
	mAnimsc->addAnimation("Dash", dashTexs);
	mAnimsc->addAnimation("Jump", jumpTexs);

	mCameraComp = new CameraComponent(this);
	mPlayerControl = new PlayerControl(this);
	mSwordComp = new SwordComponent(this);
	mArrowComp = new ArrowComponent(this);
}

void PlayerActor::input()
{
	// 基底のinput() : Componentのinput
	Actor::input();
}

void PlayerActor::update()
{
	// 基底のupdate() : Componentのupdate
	Actor::update();
}

const Camera2D& PlayerActor::getCamera() const
{
	return mCameraComp->getCamera();
}

PlayerControl& PlayerActor::getPlayerControl()
{
	return *mPlayerControl;
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}

void PlayerActor::changeState(PlayerState state)
{
	onExitState(state);
	onEnterState(state);
}

void PlayerActor::onExitState(PlayerState nextState)
{
	if (nextState <= ms_jump) {
		switch (mMoveState)
		{
		case ms_idle:
			break;
		case ms_walk:
			break;
		case ms_dash:
			break;
		case ms_jump:
			break;
		}
	}
	else {
		switch (mActionState)
		{
		case as_idle:
			break;
		case as_attack:
			mSwordComp->endAttack();
			mArrowComp->endAttack();
			break;
		case as_guard:
			break;
		}
	}
}

void PlayerActor::onEnterState(PlayerState nextState)
{
	// 使うかも
	PlayerState lastMoveState = mMoveState;
	PlayerState lastActionState = mActionState;

	if (nextState <= ms_jump) {
		mMoveState = nextState;
		switch (mMoveState)
		{
		case ms_idle:
			mAnimsc->play("Idle"); break;
		case ms_jump:
			mAnimsc->play("Jump"); break;
		case ms_walk:
			mAnimsc->play("Walk"); break;
		case ms_dash:
			mAnimsc->play("Dash"); break;
		}

	}
	else {
		mActionState = nextState;
		switch (mActionState)
		{
		case as_idle:
			break;
		case as_attack:
			// charge攻撃
			if (lastActionState == as_charge) {
				mSwordComp->startAttack();
			}
			// ダッシュ攻撃
			else if (mMoveState == ms_dash) {
				mSwordComp->startAttack();
			}
			// 通常攻撃
			else {
				//mSwordComp->startAttack(0, 9, mPlayerControl->getAttackTime());
				mArrowComp->startAttack();
			}
			break;
		case as_guard:
			break;
		}
	}
}

