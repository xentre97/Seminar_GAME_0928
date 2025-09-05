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
    void unloadData() override;
    Sequence* nextSequence() override;

    bool loadStage(const char* filename);

    void addEnemy(class EnemyActor* enemy);
    void removeEnemy(class EnemyActor* enemy);
    std::vector<class EnemyActor*> const getEnemies() { return mEnemies; }
    // 敵やその他がプレイヤー情報を取得するために定義
    class PlayerActor* getPlayer() const { return mPlayer; }

    void addWeapon(class WeaponActor* weapon, Actor::Type type);
    void removeWeapon(class WeaponActor* weapon);

    void addSprite(class SpriteComponent* sprite);
    void removeSprite(class SpriteComponent* sprite);

    const int& getStageWidth() const { return mStageWidth; }

private:
    // collision関連の更新処理がまとまってる
    void updateCollision();
    Sequence* mNext = nullptr;
    // 敵の配列(主にCollision用に使う)
    std::vector<class EnemyActor*> mEnemies;
    // 武器の配列(Collisionで使う)
    std::vector<class WeaponActor*> mPlayerWeapons;
    std::vector<class WeaponActor*> mEnemyWeapons;
    class PlayerActor* mPlayer;
    // Spriteの配列
    std::vector<class SpriteComponent*> mSprites;
    // ステージから読み込んだ四角形の配列(床や壁の描画,collisionに使用)
    std::vector<struct Rectangle> mStageRecs;
    int mStageWidth;
    int mStageHeight;
    Camera2D mCamera;
    class EnemySpawner* mSpawner;

};

// collisionの処理はcollisionManagerみたいなの作って
// 外に出してもいいかもです
