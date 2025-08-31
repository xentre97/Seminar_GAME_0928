#include "WeaponActor.h"
#include "GamePlay.h"
#include "AnimSpriteComponent.h"

#include "EnemyActor.h"
#include "PlayerActor.h"
#include "HpComponent.h"

WeaponActor::WeaponActor(Sequence* sequence, Type type, Type owner)
	: Actor(sequence, type)
	, mDamageAmt(10.0f)
{
	static_cast<GamePlay*>(mSequence)->addWeapon(this, owner);
}

WeaponActor::~WeaponActor()
{
	static_cast<GamePlay*>(mSequence)->removeWeapon(this);
}

void WeaponActor::onHit(Actor* target)
{
	//ここで動的なダメージ計算を行う。
	if (target->getType() == Actor::Eenemy) {
		if (static_cast<EnemyActor*>(target)->getHpComp()->TakeDamage(mDamageAmt)) {
			target->setState(Actor::Edead);
		}
	}
}
