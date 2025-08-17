#include "Actor.h"
#include "Sequence.h"
#include "Component.h"

Actor::Actor(Sequence* sequence)
	: mSequence(sequence)
	, mTexture()
	, mPosition(Vector2())
	, mRectangle()
{
	mSequence->addActor(this);
}

Actor::~Actor()
{
	UnloadTexture(mTexture);
	while (!mComponents.empty()) {
		delete mComponents.back();
		mComponents.pop_back();
	}
}

void Actor::input()
{
	for (auto comp : mComponents) {
		comp->input();
	}
}

void Actor::update()
{
	for (auto comp : mComponents) {
		comp->update();
	}
}

void Actor::draw()
{
}

void Actor::addComponent(Component* component)
{
	mComponents.emplace_back(component);
}
