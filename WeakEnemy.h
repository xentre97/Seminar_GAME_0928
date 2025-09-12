#pragma once
#include "EnemyActor.h"
#include "Animation.h"
#include "AttackInfo.h"
#include <unordered_map>
#include <string>

/// <summary>
/// �G���G�̃N���X�B�ȉ��̎O��ޑ��݂���
/// �E�ߐڃ^�C�v
/// �E�������^�C�v
/// �E�K�[�h�^�C�v
/// </summary>
class WeakEnemy :
    public EnemyActor
{
public:
    // �G���G�̏��
    enum EnemyState
    {
        E_walk,
        E_jump,
        E_attack,
    };

    void update() override;

    void changeState(EnemyState nextState);
    class EnemyMove* getEnemyMove() { return mEnemyMove; }
    EnemyState getEnemyState() const { return mEnemyState; }

protected:
    WeakEnemy(class Sequence* sequence);
    void computeRectangle() override;
    void onEnterState(EnemyState nextState);
    void onExitState(EnemyState nextState);
    // �X�e�[�W�Ƃ̓����蔻��
    void fixCollision();
    void jump();
    virtual Animation& getAnimation(EnemyState state) = 0;
    virtual AttackInfo& getAttackInfo() = 0;
    void computeAttackRect(Rectangle& rec);

    EnemyState mEnemyState;
    class WeaponComponent* mWeaponComp;
    class AttackComponent* mAttackComp;
    class EnemyMove* mEnemyMove;
};

class MeleeEnemy :
    public WeakEnemy
{
public:
    MeleeEnemy(class Sequence* sequence);

private:
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }
    AttackInfo mAttackInfo;
    AttackInfo& getAttackInfo() override { return mAttackInfo; }
};

class RangedEnemy :
    public WeakEnemy
{
public:
    RangedEnemy(class Sequence* sequence);

private:
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }
    AttackInfo mAttackInfo;
    AttackInfo& getAttackInfo() override { return mAttackInfo; }
};