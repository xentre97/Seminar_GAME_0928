#pragma once
#include "Actor.h"

enum class Event
{
    ActorCreated,
    ActorDestroyed,
};

class Observer
{
public:
	virtual ~Observer() {}
	virtual void onNotify(const Actor* actor, Event event) = 0;
};

