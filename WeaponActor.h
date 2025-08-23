#pragma once
#include "Actor.h"

/// <summary>
/// ïêäÌ
/// </summary>
class WeaponActor :
    public Actor
{
public:
    ~WeaponActor();
    virtual void onStartAttack(int begin, int end, float attackTime) {};

protected:
    WeaponActor(class Sequence* sequence, Type type, Type owner);
};

