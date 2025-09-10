#pragma once
#include <vector>
#include <raylib.h>

class Stage
{
public:
    Stage(class GamePlay* sequence);
    ~Stage();
	void loadStage(const char* filename);
    void update();
    void draw();

    int getStageWidth() const { return mStageWidth; }

    std::vector<struct Rectangle> getStageRecs() const { return mStageRecs; }

private:
    class GamePlay* mGamePlay;
    // �X�e�[�W����ǂݍ��񂾎l�p�`�̔z��(����ǂ̕`��,collision�Ɏg�p)
    std::vector<struct Rectangle> mStageRecs;
    int mStageWidth;
    int mStageHeight;
    std::vector<Vector2> mEnemySpawnPoints;
    class EnemySpawner* mSpawner;
};

