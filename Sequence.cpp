#include "Sequence.h"
#include "Title.h"
#include <vector>

Sequence::~Sequence()
{
	while (!mActors.empty()) {
		delete mActors.back();
		mActors.pop_back();
	}
}

void Sequence::input()
{
}

void Sequence::update()
{
}

void Sequence::draw()
{
}

void Sequence::addActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Sequence::removeActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	mActors.erase(iter);
}
