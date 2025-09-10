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
    virtual ~EnemyActor();
    class HpComponent* getHpComp() { return mHpComp; }

protected:
    EnemyActor(class Sequence* sequence);
    class HpComponent* mHpComp;
    class AnimSpriteComponent* mAnimsc;
};

