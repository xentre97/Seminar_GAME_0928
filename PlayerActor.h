#pragma once
#include "Actor.h"
#include <unordered_map>
#include "PlayerMove.h"
#include "PlayerState.h"

class PlayerState;
enum class Type;

/// <summary>
/// PlayerActor�N���X
/// </summary>
class PlayerActor :
    public Actor
{
public:
    PlayerActor(class Sequence* sequence);
    ~PlayerActor();

    void input() override;
    void update() override;

    class PlayerMove* getPlayerMove() const { return mPlayerMove; }
    class WeaponComponent* getWeapon() const { return mWeaponComp; }
    PlayerState* getPlayerState() const { return mPlayerState; }
    class AnimSpriteComponent* getAnimSpriteComp() const { return mAnimsc; }
    class HpComponent* getHpComp() { return mHpComp; }
    
    void computeRectangle() override;
    void changeState(PlayerState::Type type);

private:
    PlayerState* mPlayerState;
    std::unordered_map<PlayerState::Type, PlayerState*> mPlayerStates;
    class PlayerMove* mPlayerMove;
    class AnimSpriteComponent* mAnimsc;
    class WeaponComponent* mWeaponComp;
    class HpComponent* mHpComp;
};

// ����
// AD : �ړ�
// Space : �W�����v
// Shift + �ړ� : �_�b�V��
// S : �K�[�h(�ړ�,�W�����v�s��)
// W : �U��
