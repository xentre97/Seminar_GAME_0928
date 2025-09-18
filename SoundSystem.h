#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

/// <summary>
/// BGM,SE��炷���߂̃N���X �g�p���@�͉��̕��ɋL��
/// </summary>
class SoundSystem {
public:
    static void init() {
        if (!sInstance) sInstance = new SoundSystem();
    }
    static void shutdown() {
        delete sInstance;
        sInstance = nullptr;
    }
    static SoundSystem& instance() { return *sInstance; }

    // BGM�Đ��p
    void update();

    void loadSE(const std::string& name, const std::string& file);
    void loadBGM(const std::string& name, const std::string& file);

    void playBGM(const std::string& name, bool loop = true);
    void playSE(const std::string& name);
    void stopBGM();

private:
    static SoundSystem* sInstance;
    SoundSystem();
    ~SoundSystem();
    Music* mCurrentBGM;
    std::unordered_map<std::string, Music> mBGM;
    std::unordered_map<std::string, Sound> mSE;
    float mBGMVolume;
};

/* �g������:
 *   1.�Q�[�����Ɏg�p����BGM,SE���ꊇ���[�h(main��LoadSounds�ł���Ă܂�)
 *   �������ɂ͎Q�Ɨp�̖��O��n��
 *   SoundSystem &s = SoundSystem::instance();
 *   s.loadBGM("stageBGM", "Assets/filename");�@
 * �@s.loadSE("explosionSE", "Assets/filename");
 *   ...
 *   �K�v��SE,BGM�����[�h����
 * 
 *   2.�Q�Ɨp�̖��O���g���ĉ����Ȃ炷
 *   SE...s.playSE("explosionSE");
 *   BGM...s.playBGM("stageBGM");
 * 
 *   SoundSystem::update()��main�ŌĂяo���Ă���̂ł���ȏ�̌Ăяo���͕s�v�ł�
 *   Sequence�ŌĂԂ̂��l���܂�����,main�̕������N����ɂ������Ȃ̂�
 * 
 *   �ȉ��֑�
 *   �E���z�̓f�[�^�쓮��,JSON�t�@�C���ɎQ�Ɨp�̖��O�ƃt�@�C�������܂Ƃ߂�
 *   �ǂݍ��ނƂ��o�����������
 *   �EAssets/BGM�ȉ��̃t�@�C����S�ǂݍ��݂���...�Ƃ��͏o�������ł͂���
 */