#include "AttackComponent.h"

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
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // 現在の攻撃を設定
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

	// ダメージ与える
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
