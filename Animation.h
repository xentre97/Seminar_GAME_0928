#pragma once
#include <vector>
#include <raylib.h>

/// <summary>
/// �A�j���[�V�������
/// </summary>
struct Animation
{
    std::vector<Texture2D*> frames;    // �e�t���[���̃e�N�X�`��
    float fps = 24.0f;                 // �Đ�FPS
    bool loop;                         // ���[�v�A�j���[�V�������ǂ���
};