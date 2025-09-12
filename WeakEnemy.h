#pragma once
#include "EnemyActor.h"
#include "Animation.h"
#include "AttackInfo.h"
#include <unordered_map>
#include <string>

/// <summary>
/// 雑魚敵のクラス。以下の三種類存在する
/// ・近接タイプ
/// ・遠距離タイプ
/// ・ガードタイプ
/// </summary>
class WeakEnemy :
    public EnemyActor
{
public:
    // 雑魚敵の状態
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
    // ステージとの当たり判定
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