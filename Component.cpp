#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner)
	: mOwner(owner)
{
	mOwner->addComponent(this);
}

Component::~Component()
{
}

void Component::update()
{
}
