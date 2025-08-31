#pragma once
#include "WeaponComponent.h"

/// <summary>
/// 近接武器：剣(名前は間に合わせ)
/// 近接武器自身にアタッチするわけではない。ややこしいので名前変えてもいいかも
/// </summary>
class SwordComponent :
    public WeaponComponent
{
public:
    SwordComponent(class Actor* owner);
    void update() override;
    void startAttack() override;

private:
    void updateWeaponPosition();
};

