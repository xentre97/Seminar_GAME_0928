#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"
#include "Boss.h"

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
		/* �ｽU�ｽ�ｽ�ｽI�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ */
		mTimer += GetFrameTime();
		// �ｽU�ｽ�ｽ�ｽ�ｽ�ｽﾔゑｿｽ�ｽﾟゑｿｽ�ｽ�ｽ�ｽ�ｽ�ｽactive�ｽ�ｽ
		if (mTimer > mCurInfo->duration) {
			mActive = false;
		}
	
		switch (mCurInfo->targetType)
		{
		// �ｽG�ｽﾖの攻�ｽ�ｽ�ｽﾌ場合
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
	// �ｽ謦｣�ｽ�ｽ�ｽﾄ敵�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾄゑｿｽ�ｽ�ｽB�ｽ{�ｽ�ｽ�ｽ�ｽActorManager�ｽ�ｽ�ｽ邇晢ｿｽ�ｽ�ｽﾄゑｿｽ�ｽ�ｽﾝゑｿｽ�ｽ�ｽ�ｽﾈ仕�ｽg�ｽﾝにゑｿｽ�ｽ�ｽ�ｽ�ｽ�ｽ@
	std::vector<EnemyActor*> enemies = 
		static_cast<GamePlay*>(mOwner->getSequence())->getEnemies();
	for (auto enemy : enemies) {
		// �ｽ�ｽ�ｽ�ｽ �ｽF�ｽX�ｽ�ｽ竄ｪ�ｽ�ｽ�ｽ�ｽ
		//
		// 1.�ｽm�ｽb�ｽN�ｽo�ｽb�ｽN�ｽ�ｽﾔゑｿｽ�ｽ�ｽﾈゑｿｽ�ｽG�ｽﾉは難ｿｽ�ｽﾄなゑｿｽ
		//  �ｽﾈ単�ｽ�ｽ�ｽ�ｽ,�ｽm�ｽb�ｽN�ｽo�ｽb�ｽN�ｽ�ｽ�ｽﾔゑｿｽ�ｽZ�ｽ�ｽ�ｽ�ｽ�ｽﾆゑｿｽ�ｽO�ｽ�ｽB�ｽ�ｽ閧�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ轤ｵ�ｽ�ｽ�ｽﾈゑｿｽC�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ
		// 2.�ｽq�ｽb�ｽg�ｽ�ｽ�ｽX�ｽg
		//  �ｽ�ｽﾔ暦ｿｽ�ｽz�ｽﾅはゑｿｽ�ｽ�ｽB�ｽG�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽo�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾈゑｿｽO(n^2)�ｽ�ｽ�ｽC�ｽﾉなるか�ｽ�ｽ(�ｽ�ｽ�ｽ�ｽ�ｽ蛯ｵ�ｽﾄ変ゑｿｽ�ｽﾈゑｿｽ�ｽ�ｽ�ｽﾍゑｿｽ�ｽ�ｽ)
		//  �ｽﾇ記�ｽF�ｽq�ｽb�ｽg�ｽ�ｽ�ｽX�ｽg�ｽ�ｽ�ｽﾌ敵�ｽ�ｽdelete�ｽ�ｽ�ｽ黷ｽ�ｽ�ｽ�ｽﾌ擾ｿｽ�ｽ�ｽ�ｽ�ｽ�ｽK�ｽv
		// 3.�ｽU�ｽ�ｽ�ｽt�ｽ�ｽ�ｽ[�ｽ�ｽ�ｽ�ｽ�ｽﾉ誰�ｽ�ｽ�ｽﾉ難ｿｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ,mActive = false�ｽﾉゑｿｽ�ｽ�ｽ
		//  �ｽ皷橸ｿｽ�ｽ�ｽ�ｽ�ｽﾉ包ｿｽ�ｽ�ｽ�ｽﾉ難ｿｽ�ｽﾄゑｿｽ�ｽ�ｽﾌゑｿｽ�ｽ�ｽ�ｽ�ｽ,�ｽm�ｽb�ｽN�ｽo�ｽb�ｽN�ｽ�ｽ�ｽﾌ敵�ｽﾉゑｿｽ�ｽ�ｽ�ｽﾄゑｿｽ�ｽ�ｽ
		//  �ｽc�ｽ闕�ｿｽI�ｽﾈ攻�ｽ�ｽ�ｽ�ｽ�ｽ閧ｪ�ｽﾈゑｿｽ�ｽﾌで、�ｽ_�ｽb�ｽV�ｽ�ｽ�ｽU�ｽ�ｽ�ｽﾌ具ｿｽ�ｽ�ｽ�ｽﾍ心�ｽz

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
		
    // コンフリクト解消の都合で,とりあえず
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			if (auto boss = dynamic_cast<Boss*>(enemy)) {
				// 笘 Boss 縺縺代�繧ｬ繝ｼ繝峨ｒ閠��縺励◆繝繝｡繝ｼ繧ｸ驕ｩ逕ｨ
				boss->ApplyDamage(mCurInfo->damage, mCurInfo->tag);
				// �亥偵ｌ蛻､螳壹�Boss蛛ｴ/HP蛛ｴ縺ｧ蜃ｦ逅�ゅ％縺薙〒setState縺ｯ荳崎ｦ�ｼ
			}
		}

		// �ｽﾈゑｿｽ�ｽA�ｽG�ｽ�ｽ�ｽG�ｽﾉ攻�ｽ�ｽ�ｽ�ｽ�ｽ骼厄ｿｽ�ｽ�ｽﾂ能�ｽﾈはゑｿｽ
		// �ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽg�ｽ�ｽ�ｽU�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾈゑｿｽ�ｽﾈゑｿｽ,component�ｽ�ｽowner�ｽﾆは難ｿｽ�ｽ�ｽ�ｽ�ｽﾈゑｿｽ�ｽ謔､�ｽﾉゑｿｽ�ｽ�ｽ�ｽﾉ擾ｿｽ�ｽ�ｽ�ｽﾎゑｿｽ�ｽ�ｽ
		// �ｽm�ｽb�ｽN�ｽo�ｽb�ｽNenter�ｽ�ｽ�ｽﾉ敵�ｽ�ｽstartAttack()�ｽ�ｽ�ｽﾄべゑｿｽ,�ｽG�ｽﾆ敵�ｽﾌノ�ｽb�ｽN�ｽo�ｽb�ｽN�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾂ能�ｽ�ｽ�ｽ�､
	}
}

void AttackComponent::processAttackPlayer()
{
	// �ｽ�ｽ�ｽm�ｽ�ｽplayer�ｽﾌ取得
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
