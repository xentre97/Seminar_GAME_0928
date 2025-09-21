#pragma once
#include "Actor.h"

/// <summary>
/// �A�C�e���̊��N���X
/// �v���C���[�ɓ�����Ɖe�����y�ڂ�
/// </summary>
class ItemActor :
    public Actor
{
public:
    ItemActor(class Sequence* sequence);
    virtual ~ItemActor() {};

    void update() override;
    void computeRectangle() override;
    // �v���C���[���G�ꂽ����
    virtual void onAcquired() = 0;

protected:
    class SpriteComponent* mSpriteComp;
};

/// <summary>
/// �񕜃A�C�e��
/// </summary>
class HealingItem
    : public ItemActor
{
public:
    HealingItem(class Sequence* sequence);
    void onAcquired() override;
private:
    float mHealAmount;
};

/// <summary>
/// �ړ����x�㏸�A�C�e��
/// </summary>
//class SpeedUpItem
//    : ItemActor
//{
//public:
//
//
//};