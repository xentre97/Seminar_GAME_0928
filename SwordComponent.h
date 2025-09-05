#pragma once
#include "WeaponComponent.h"

/// <summary>
/// �ߐڕ���F��(���O�͊Ԃɍ��킹)
/// �ߐڕ��펩�g�ɃA�^�b�`����킯�ł͂Ȃ��B��₱�����̂Ŗ��O�ς��Ă���������
/// </summary>
class SwordComponent :
    public WeaponComponent
{
public:
    SwordComponent(class Actor* owner);
    void update() override;
    void startAttack(AttackType type = AttackType::Normal) override;

private:
    void updateWeaponPosition();
};

