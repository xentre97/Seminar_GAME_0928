#pragma once
#include "Component.h"
#include <raylib.h>

/// <summary>
/// 武器Component基底クラス
/// 武器自身にアタッチするわけではない。ややこしいので名前変えてもいいかも
/// </summary>
class WeaponComponent :
    public Component
{
public:
    virtual void update() override;
    virtual void startAttack() {};
    virtual void endAttack();

protected:
    WeaponComponent(class Actor* owner);
    class WeaponActor* mWeapon;
};

