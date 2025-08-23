#include "WeaponComponent.h"
#include "WeaponActor.h"

WeaponComponent::WeaponComponent(Actor* owner)
	: Component(owner)
{
}

void WeaponComponent::update()
{
}

void WeaponComponent::startAttack(int begin, int end, float attackTime)
{
}

void WeaponComponent::endAttack()
{
	if (mWeapon) {
		mWeapon->setState(Actor::Edead);
		mWeapon = nullptr;
	}
}
