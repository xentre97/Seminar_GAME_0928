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
	// �U���͂��v�Z �����_���v�f�������Ă��悳����
	float damageAmount = mAttackPower * mAttackMultiplier;
	//�����œ��I�ȃ_���[�W�v�Z���s���B
	if (target->getType() == Actor::Eenemy) {
		if (static_cast<EnemyActor*>(target)->getHpComp()->TakeDamage(damageAmount)) {
			target->setState(Actor::Edead);
		}
	}
	else if (target->getType() == Actor::Eplayer) {
		static_cast<PlayerActor*>(target)->getHpComp()->TakeDamage(damageAmount);
	}
}
