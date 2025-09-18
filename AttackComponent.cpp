#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

#include "HpComponent.h"

AttackComponent::AttackComponent(Actor* owner)
	: Component(owner)
	, mTimer(0.0f)
	, mActive(false)
	, mCurInfo(nullptr)
{
}

void AttackComponent::update()
{
	if (mActive) {
		/* 攻撃終了判定 */
		mTimer += GetFrameTime();
		// 攻撃時間を過ぎたら非activeに
		if (mTimer > mCurInfo->duration) {
			mActive = false;
		}
	
		switch (mCurInfo->targetType)
		{
		// 敵への攻撃の場合
		case Actor::Type::Eenemy:
			processAttackEnemy();
			break;
		case Actor::Type::Eplayer:
			processAttackPlayer();
			break;
		}
		//kb move
		for (int i = mKnockbackTargets.size() - 1; i >= 0; --i) {
			auto& info = mKnockbackTargets[i];

			//ターゲットが有効かチェック
			if (info.target->getState() == Actor::Edead) {
				mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
				continue;
			}
			info.timer -= GetFrameTime();

			if (info.timer <= 0.0f) {
				mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
			}
			else {
				Vector2 currentPos = info.target->getPosition();
				Vector2 moveAmount = Vector2Scale(info.velocity, GetFrameTime());
				Vector2 newPos = Vector2Add(currentPos, moveAmount);

				info.target->setPosition(newPos);
				info.target->computeRectangle();
			}
		}
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // 現在の攻撃を設定
	mHitActors.clear();
}

void AttackComponent::processAttackEnemy()
{
	// 頑張って敵を持ってくる。本当はActorManagerから持ってくるみたいな仕組みにしたい　
	std::vector<EnemyActor*> enemies = 
		static_cast<GamePlay*>(mOwner->getSequence())->getEnemies();
	for (auto enemy : enemies) {
		// 処理 色々候補がある
		//
		// 1.ノックバック状態じゃない敵には当てない
		//  簡単だが,ノックバック時間が短いことが前提。わりあいそれらしくなる気がする
		// 2.ヒットリスト
		//  一番理想ではある。敵が多く出現させるならO(n^2)が気になるかも(他も大して変わらない説はある)
		//  追記：ヒットリスト内の敵がdeleteされた時の処理が必要
		// 3.攻撃フレーム中に誰かに当たったら,mActive = falseにする
		//  一応同時に複数に当てられるのが自慢,ノックバック中の敵にも当てられる
		//  残り香的な攻撃判定がないので、ダッシュ攻撃の挙動は心配

		// ノックバック状態がまだないので,とりあえず3を採用しました
		if (enemy->getHpComp()->isInvincible()) {
			continue;
		}
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			mHitActors.push_back(enemy);
			//Kb
			Vector2 attackerPos = mOwner->getPosition();
			Vector2 targetPos = enemy->getPosition();
			Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

			float upspeed = 2.0f;
			direction.y -= upspeed;
			direction = Vector2Normalize(direction);

			KnockbackInfo info;
			info.target = enemy; //Knockback構造体のtargetにenemyを設定
			info.timer = 0.2f;
			float speed = 300.0f;
			info.velocity = Vector2Scale(direction, speed);

			mKnockbackTargets.push_back(info);

			if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
				mActive = false;
			}
		}
		
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
				mActive = false;
			}
		}

		// なお、敵が敵に攻撃する事も可能なはず
		// 自分自身を攻撃したくないなら,componentのownerとは当たらないようにここに書けばいい
		// ノックバックenter時に敵がstartAttack()を呼べば,敵と敵のノックバック処理も可能だろう
	}
}

void AttackComponent::processAttackPlayer()
{
	// 滑稽なplayerの取得
	PlayerActor* player = static_cast<GamePlay*>(mOwner->getSequence())->getPlayer();

	if (player->getHpComp()->isInvincible()) {
		return;
	}
	// ダメージ与える
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		mHitActors.push_back(player);
		Vector2 attackerPos = mOwner->getPosition();
		Vector2 targetPos = player->getPosition();
		Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

		float flyspeed = 0.2f;
		direction.y -= flyspeed;
		direction = Vector2Normalize(direction);
		KnockbackInfo info;
		info.target = player;
		info.timer = 0.2f;
		float speed = 150.0f;
		info.velocity = Vector2Scale(direction, speed);

		mKnockbackTargets.push_back(info);

		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
