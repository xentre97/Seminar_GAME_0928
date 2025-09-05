#pragma once
#include "Actor.h"

/// <summary>
/// ����
/// </summary>
class WeaponActor :
    public Actor
{
public:
    ~WeaponActor();
    virtual void onStartAttack() {};
    virtual void onHit(class Actor* target);

    void setMultiplier(float multiplier) { mAttackMultiplier = multiplier; }

protected:
    WeaponActor(class Sequence* sequence, Type owner);
    float mAttackPower;         // ��b�U����
    float mAttackMultiplier;    // ��b�U���͂���Z�ő���������
    // �^����_���[�W�͈ȉ��̗l�ɂȂ�
    // mAttackPower * mAttackMultiplier
};

