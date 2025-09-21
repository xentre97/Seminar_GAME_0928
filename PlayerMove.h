#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player�̈ړ����i��
/// Player��State��ύX�����������
/// </summary>
class PlayerMove :
    public MoveComponent
{
public:
    PlayerMove(class PlayerActor* owner);
    void input()override;
    void update()override;
    void fixFloorCol();
    void fixCeilingCol();

    // �W�����v����u�ԂɌĂ�
    void jump();

private:
    void StateDraw(); // �f�o�b�O�p�̊֐�
    class PlayerActor* mPlayer; // mOwner�Ɠ���(�^���Ⴄ)
};
