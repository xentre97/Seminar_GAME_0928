#pragma once
#include "UIScreen.h"
#include <raylib.h>
#include <vector>

struct DamageNumber {
    bool active;
    int amount;
    Vector2 position;
    float timer;
};

/// <summary>
/// �U���q�b�g���ɕ\������_���[�W�ʂ�ui
/// </summary>
class DamageUI :
    public UIScreen
{
public:
    DamageUI(class GamePlay* sequence);

    void update() override;
    void draw() override;

    // �_���[�W�ʂƔ����ʒu���󂯎��
    void addDamage(int amount, Vector2 worldPos);

private:
    class GamePlay* mGamePlaySeq;   // �L���X�g�ʓ|
    std::vector<DamageNumber> mDamagePool;
    int mVelocityY;  // �_���[�W�\�L���㏸���鑬�x
};

