#pragma once
#include "Animation.h"
#include "AttackInfo.h"

class PlayerActor;

/// <summary>
/// �v���C���[�̏�ԋ@�B�̊��
/// </summary>
class PlayerState
{
public:
    enum class Type
    {
        Idle,
        Walk,
        Jump,
        Guard,
        Dodge,
        Charge,
        NormalAttack,
        DodgeAttack,
        ChargeAttack,
    };
    virtual ~PlayerState() {}
    virtual void enter();       // ��Ԃɓ��������ɌĂяo��
    virtual void exit() {}      // ��Ԃ��o�鎞�ɌĂяo��
    virtual void input() {}     // ����
    virtual void update() {}    // �X�V
    virtual void init() {}      // ��Ԃ����Z�b�g����

    Type getType() const { return mType; }
    void setAnimation(Animation anim) { mAnim = anim; }
protected:
    // �R���X�g���N�^�Ńv���C���[������
    PlayerState(PlayerActor* player, Type type);
    PlayerActor* mPlayer;
    const Type mType;
    Animation mAnim;
    // �U����Ԃ݂̂Ŏg��,�U���͈͂̈ʒu���v�Z����
    void computeAttackRectPos(Rectangle &rec);
};

// Idle
class Idle
    : public PlayerState
{
public:
    Idle(PlayerActor* player);
    void input() override;
    void enter() override;
private:
    double mLastPressedTimeD;
    double mLastPressedTimeA;
    double mDoubleTapWindow;     // ����̍ۂ�2����͂̎�t�P�\����
};

// ����
class Walk
    : public PlayerState
{
public:
    Walk(PlayerActor* player);
    void input() override;
    void update() override;
private:
    float mWalkSpeed;
};

// �W�����v
class Jump
    : public PlayerState
{
public:
    Jump(PlayerActor* player);
    void enter() override;
    void update() override;
private:
    float mHorizontalSpeed; // �W�����v���̉��ړ����x
};

// �K�[�h
class Guard
    : public PlayerState
{
public:
    Guard(PlayerActor* player);
};

// ���
class Dodge
    : public PlayerState
{
public:
    Dodge(PlayerActor* player);
    void update() override;
    void enter() override;
private:
    float mDodgeTime;
    float mDodgeTimer;
    float mDodgeSpeed;  // ��𒆂̈ړ����x
};

// ����
class Charge
    : public PlayerState
{
public:
    Charge(PlayerActor* player);
    void input() override;
    void update() override;
private:
    float mChargeTimer;
    float mChargeTime;
};

// �ʏ�U��
class NormalAttack
    : public PlayerState
{
public:
    NormalAttack(PlayerActor* player);
    void update() override;
    void enter() override;
private:
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};

class DodgeAttack
    : public PlayerState
{
public:
    DodgeAttack(PlayerActor* player);
    void update() override;
    void enter() override;
    void exit() override;
private:
    float mHorizontalSpeed;
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};

class ChargeAttack
    : public PlayerState
{
public:
    ChargeAttack(PlayerActor* player);
    void update() override;
    void enter() override;
private:
    float mAttackTime;
    float mAttackTimer;
    AttackInfo mAttackInfo;
};


// ���
// idle
// walk
// jump
// dodge
// charge
// normalAttack
// dodgeAttack
// chargeAttack