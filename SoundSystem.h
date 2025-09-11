#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

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

    // BGMçƒê∂óp
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
};

