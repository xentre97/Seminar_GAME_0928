#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "Actor.h"

/// <summary>
/// �V�[�P���X�̐e�N���X
/// �^�C�g��,�Q�[�����,�Q�[���I�[�o�[�Ȃǂ��q�Ɏ���
/// 
/// </summary>
class Sequence
{
public:
    virtual ~Sequence();

    virtual void input();
    virtual void update();
    virtual void draw();

    virtual void unloadData();

    // �V�[���J�ڂ��K�v�ɂȂ����玟�̃V�[�P���X��Ԃ�
    virtual Sequence* nextSequence() { return nullptr; }

    void addActor(Actor* actor);
    void removeActor(Actor* actor);

    void pushUI(class UIScreen* screen);

    Texture2D* getTexture(const std::string& fileName);

protected:
    // ���̃V�[�P���X�ɑ��݂���A�N�^�[�̔z��
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors; // mActors�ւ̒ǉ��҂���Actor
    // ���̃V�[�P���X�ŕ\������UI�̃X�^�b�N
    std::vector<class UIScreen*> mUIStack;
    // ���̃V�[�P���X�Ń��[�h�����e�N�X�`���B
    std::unordered_map<std::string, Texture2D> mTextures;
    bool mUpdatingActors = false;
    // �R���X�g���N�^��protected�ɂ��Ē��� new Sequence�ł��Ȃ��悤��
    Sequence() {};
};
