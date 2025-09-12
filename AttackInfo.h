#pragma once
#include <raylib.h>
#include <vector>
#include "Actor.h"

struct AttackInfo
{
	float damage;		// �_���[�W��
	float duration;		// �U���������� �A�j���[�V�����Ƃ͓Ɨ����ĊǗ�����
	Rectangle colRect;	// �U���͈� �����蔻��p�̋�`
	float knockBack;	// �m�b�N�o�b�N��
	int forward;		// �U���̌���

	Actor::Type targetType;   // �U���Ώۂ̎��
};