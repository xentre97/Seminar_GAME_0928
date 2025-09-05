#include "SwordComponent.h"
#include "SwordActor.h"

SwordComponent::SwordComponent(Actor* owner)
	: WeaponComponent(owner)
{
}

void SwordComponent::update()
{
	if (mWeapon) {
		updateWeaponPosition();
	}
}

void SwordComponent::startAttack(AttackType type)
{
	mWeapon = new SwordActor(mOwner->getSequence(), mOwner->getType());
	mWeapon->onStartAttack();
	// UŒ‚‚ÌŽí—Þ‚É‚æ‚Á‚ÄUŒ‚—Í‘‰Á‚Ì”{—¦‚ðÝ’è‚·‚é
	// Œ»ó‚ÌÝŒv‚Å‚Í,•ŠíŽí‚ÉŒÅ—L‚Æ‚È‚é
	float multiplier = 1.0f;
	switch (type)
	{
	case AttackType::Dodge: multiplier = 1.1f; break;
	case AttackType::Charge: multiplier = 1.5f; break;
	}
	mWeapon->setMultiplier(multiplier);
	updateWeaponPosition();
	mWeapon->computeRectangle();
}

void SwordComponent::updateWeaponPosition()
{
	// •ŠíŠ—LŽÒ‚Ì‘O•û‚É•Ší‚ÌˆÊ’u‚ðÝ’è
	Vector2 pos = mOwner->getPosition();
	mWeapon->setForward(mOwner->getForward());
	pos.x += (mOwner->getRectangle().width + mWeapon->getRectangle().width)
		* mWeapon->getForward() / 2.0f;
	mWeapon->setPosition(pos);
}
