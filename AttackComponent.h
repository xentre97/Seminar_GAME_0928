#pragma once
#include "Component.h"
#include "AttackInfo.h"

/// <summary>
/// �U�����s��(HP�����炷,�m�b�N�o�b�N�t�^,���ʉ��炷)�@�\��t�^����component
/// �O������AttackInfo���󂯎��A����Ɋ�Â�������������
/// </summary>
class AttackComponent :
    public Component
{
public:
    AttackComponent(class Actor* owner);

    void update() override;
    // �U������ۂɌĂяo��,�U�����ɂ��Ăׂ�
    void startAttack(AttackInfo* info);

private:
    void processAttackEnemy();
    void processAttackPlayer();
    
    float mTimer;
    // ���݂̍U�����
    AttackInfo* mCurInfo;
    bool mActive;
};

