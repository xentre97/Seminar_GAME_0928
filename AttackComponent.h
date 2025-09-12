#pragma once
#include "Component.h"
#include "AttackInfo.h"

/// <summary>
/// �U�����s���@�\��t�^����component
/// �O������AttackInfo���󂯎��A����Ɋ�Â�������������
/// </summary>
class AttackComponent :
    public Component
{
public:
    AttackComponent(class Actor* owner);

    void update() override;
    // �U������ۂɌĂяo��,�U�����ɂ��Ăׂ�
    void startAttack(AttackInfo& info);

private:
    void processAttackEnemy();
    void processAttackPlayer();
    
    float mTimer;
    // ���݂̍U�����
    AttackInfo* mCurInfo;
    bool mActive;
};

