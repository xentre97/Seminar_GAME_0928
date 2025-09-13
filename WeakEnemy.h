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

    void changeState(EnemyState nextState);
    class EnemyMove* getEnemyMove() { return mEnemyMove; }
    EnemyState getEnemyState() const { return mEnemyState; }

protected:
    WeakEnemy(class Sequence* sequence);
    void computeRectangle() override;
    virtual void onEnterState(EnemyState nextState);
    virtual void onExitState(EnemyState nextState);
    // �X�e�[�W�Ƃ̓����蔻��
    void fixCollision();
    void jump();
    // AttackComponent��startAttack���Ă�or����(��Ƃ�)���o�������铙�̖���
    virtual void attack() {};
    virtual Animation& getAnimation(EnemyState state) = 0;
    void computeAttackRectPos(Rectangle& rec);

    EnemyState mEnemyState;
    class EnemyMove* mEnemyMove;
};

class MeleeEnemy :
    public WeakEnemy
{
public:
    MeleeEnemy(class Sequence* sequence);
    void update() override;

private:
    void attack() override;
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }
    class AttackComponent* mAttackComp;
    AttackInfo mAttackInfo;
};

class RangedEnemy :
    public WeakEnemy
{
public:
    RangedEnemy(class Sequence* sequence);
    void update() override;

private:
    void onEnterState(EnemyState nextState) override;
    static std::unordered_map<EnemyState, Animation> mAnimations;
    Animation& getAnimation(EnemyState state) override { return mAnimations[state]; }

    std::vector<class ArrowActor*> mArrows;
};