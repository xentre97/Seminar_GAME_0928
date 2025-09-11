#include "WeaponActor.h"
#include "GamePlay.h"
#include "AnimSpriteComponent.h"

#include "EnemyActor.h"
#include "PlayerActor.h"
#include "HpComponent.h"

WeaponActor::WeaponActor(Sequence* sequence, Type owner)
	: Actor(sequence, Type::Eweapon)
	, mAttackPower(10.0f)
	, mAttackMultiplier(1.0f)
{
}

WeaponActor::~WeaponActor()
{
}

void WeaponActor::onHit(Actor* target)
{
	// 攻撃力を計算 ランダム要素があってもよさそう
	float damageAmount = mAttackPower * mAttackMultiplier;
	//ここで動的なダメージ計算を行う。
	if (target->getType() == Actor::Eenemy) {
		if (static_cast<EnemyActor*>(target)->getHpComp()->TakeDamage(damageAmount)) {
			target->setState(Actor::Edead);
		}
	}
	else if (target->getType() == Actor::Eplayer) {
		static_cast<PlayerActor*>(target)->getHpComp()->TakeDamage(damageAmount);
	}
}
