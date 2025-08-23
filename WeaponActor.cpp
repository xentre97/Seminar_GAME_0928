#include "WeaponActor.h"
#include "GamePlay.h"
#include "AnimSpriteComponent.h"

WeaponActor::WeaponActor(Sequence* sequence, Type type, Type owner)
	: Actor(sequence, type)
{
	static_cast<GamePlay*>(mSequence)->addWeapon(this, owner);
}

WeaponActor::~WeaponActor()
{
	static_cast<GamePlay*>(mSequence)->removeWeapon(this);
}
