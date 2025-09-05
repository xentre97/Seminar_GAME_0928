#pragma once
#include "Actor.h"

/// <summary>
/// �G�A�N�^�[
/// �K�v�ɉ����Ă܂��p������
/// </summary>
class EnemyActor :
    public Actor
{
public:
    // Player�ƈ���Ĉړ����Ȃ���U���Ȃǂ͍l���Ȃ��̂�,��̃X�e�[�g�ɂ܂Ƃ߂�
    enum EnemyState
    {
        E_walk,
        E_jump,
        E_attack,
    };
    EnemyActor(class Sequence* sequence);
    ~EnemyActor();

    void update() override;
    void computeRectangle() override;

    void jump();
    void changeState(EnemyState nextState);

    class EnemyMove* getEnemyMove() { return mEnemyMove; }
    class HpComponent* getHpComp() { return mHpComp; }
    EnemyState getEnemyState() const { return mEnemyState; }


private:
    void onEnterState(EnemyState nextState);
    void onExitState(EnemyState nextState);
    EnemyState mEnemyState;
    class SpriteComponent* mSpriteComp;
    class EnemyMove* mEnemyMove;
    class HpComponent* mHpComp;
    class WeaponComponent* mWeaponComp;
};

