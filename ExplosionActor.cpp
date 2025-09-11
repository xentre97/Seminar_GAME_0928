#include "ExplosionActor.h"
#include "Animation.h"
#include "AnimSpriteComponent.h"
#include "GamePlay.h"

#include "PlayerActor.h"
#include "EnemyActor.h"
#include "StageObject.h"
#include "HpComponent.h"

Animation ExplosionActor::mAnim = Animation{ {}, 24.0f, true };

ExplosionActor::ExplosionActor(GamePlay* sequence)
	: Actor(sequence, Type::Eexplosion)
	, mGamePlay(sequence)
	, mDamageAmt(30.0f)
	, mTimer(0.0f)
	, mActiveTime(0.5f)
{
	mAnimsc = new AnimSpriteComponent(this);
	mAnim.frames = { mSequence->getTexture("Assets/testBomb.png") };
	mAnim.loop = false;
	mAnimsc->play(&mAnim);
}

void ExplosionActor::update()
{
	Actor::update();

	mTimer += GetFrameTime();
	if (mTimer > mActiveTime) {
		setState(Edead);
		return;
	}

	/* 敵やプレイヤーとのコリジョンチェック */
	PlayerActor* player = mGamePlay->getPlayer();
	Rectangle targetRec = player->getRectangle();
	if (!player->getHpComp()->isInvincible()) {
		if (CheckCollisionRecs(targetRec, mRectangle)) {
		
			player->getHpComp()->TakeDamage(mDamageAmt);
		}
	}
	
	for (auto enemy : mGamePlay->getEnemies()) {
		if (!enemy->getHpComp()->isInvincible()) {
			targetRec = enemy->getRectangle();
			if (CheckCollisionRecs(targetRec, mRectangle)) {
				if (enemy->getHpComp()->TakeDamage(mDamageAmt)) {
					enemy->setState(Actor::Edead);
				};
			}
		}
	}

	for (auto& obj : mGamePlay->getStageObjs()) {
		if (!obj->getHpComp()->isInvincible()) {
			targetRec = obj->getRectangle();
			if (CheckCollisionRecs(targetRec, mRectangle)) {
				obj->getHpComp()->TakeDamage(mDamageAmt);
			}
		}
	}
}

void ExplosionActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
	mRectangle.width = mAnimsc->getTexWidth();
	mRectangle.height = mAnimsc->getTexHeight();
}
