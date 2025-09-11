#include "SoundSystem.h"

SoundSystem* SoundSystem::sInstance = nullptr;

SoundSystem::SoundSystem()
    : mCurrentBGM(nullptr)
{
    InitAudioDevice();
}

SoundSystem::~SoundSystem()
{
    for (auto& kv : mSE) UnloadSound(kv.second);
    for (auto& kv : mBGM) UnloadMusicStream(kv.second);
    CloseAudioDevice();
}

void SoundSystem::loadSE(const std::string& name, const std::string& file)
{
    mSE[name] = LoadSound(file.c_str());
}

void SoundSystem::loadBGM(const std::string& name, const std::string& file)
{
    mBGM[name] = LoadMusicStream(file.c_str());
}

void SoundSystem::playSE(const std::string& name)
{
    if (mSE.count(name)) PlaySound(mSE[name]);
}

void SoundSystem::playBGM(const std::string& name, bool loop)
{
    stopBGM();
    mCurrentBGM = &mBGM[name];
    PlayMusicStream(*mCurrentBGM);
    mCurrentBGM->looping = loop;
}

void SoundSystem::stopBGM()
{
    if (mCurrentBGM) {
        StopMusicStream(*mCurrentBGM);
        mCurrentBGM = nullptr;
    }
}

void SoundSystem::update()
{
    if (mCurrentBGM) {
        UpdateMusicStream(*mCurrentBGM);
    }
}
