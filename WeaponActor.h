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
    virtual void onStartAttack() {};
    virtual void onHit(class Actor* target);

protected:
    WeaponActor(class Sequence* sequence, Type type, Type owner);
    float mDamageAmt;
};

