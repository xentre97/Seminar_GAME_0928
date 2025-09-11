#pragma once
#include "Component.h"
#include <raylib.h>

enum class AttackType
{
    Normal,
    Charge,
    Dodge
};

/// <summary>
/// ����Component���N���X
/// ���펩�g�ɃA�^�b�`����킯�ł͂Ȃ��B��₱�����̂Ŗ��O�ς��Ă���������
/// </summary>
class WeaponComponent :
    public Component
{
public:
    virtual ~WeaponComponent();
    virtual void update() override;
    virtual void startAttack(AttackType type = AttackType::Normal) {};
    virtual void endAttack();
    class WeaponActor* getWeapon() const { return mWeapon; }

protected:
    WeaponComponent(class Actor* owner);
    class WeaponActor* mWeapon;
};

