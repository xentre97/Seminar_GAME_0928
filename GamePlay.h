#pragma once
#include "Sequence.h"

/// <summary>
/// GamePlayシーケンス
/// </summary>
class GamePlay :
    public Sequence
{
public:
    GamePlay();
    ~GamePlay() override;

    void input() override;
    void update() override;
    void draw() override;
    Sequence* nextSequence() override;

    bool loadStage(const char* filename);

    void addEnemy(class EnemyActor* enemy);
    std::vector<class EnemyActor*> const getEnemies() { return mEnemies; }

    const int& getStageWidth() const { return mStageWidth; }

private:
    void destroyEnemy(class EnemyActor* enemy);
    // collision関連の更新処理がまとまってる
    void updateCollision();
    Sequence* mNext = nullptr;
    // 敵の配列(主にCollision用に使う)
    std::vector<class EnemyActor*> mEnemies;
    class PlayerActor* mPlayer;
    // ステージから読み込んだ四角形の配列(床や壁の描画,collisionに使用)
    std::vector<struct Rectangle> mStageRecs;
    int mStageWidth;
    int mStageHeight;
    Camera2D mCamera;
};

// collisionの処理はcollisionManagerみたいなの作って
// 外に出してもいいかもです
