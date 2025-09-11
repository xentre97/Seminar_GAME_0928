#include "Stage.h"
#include <fstream>
#include <string>
#include <vector>
#include <raymath.h>

#include "GamePlay.h"
#include "PlayerActor.h"
#include "EnemySpawner.h"
#include "StageObject.h"

Stage::Stage(GamePlay* sequence)
    : mGamePlay(sequence)
    , mStageHeight(0)
    , mStageWidth(0)
{
    mSpawner = new EnemySpawner(sequence);
}

Stage::~Stage()
{
    delete mSpawner;
    mSpawner = nullptr;
}

void Stage::loadStage(const char* filename)
{
    // ステージファイルの読み込み
    // ' ' : 空白(通行可能)
    // # : ブロック(壁)
    // w : 木の板
    // E : 敵の出現位置
    // B : ボスエリアの入口 ここにプレイヤーが接触したらボスステージの読み込みを始める
    // 今はとりあえずシンプルな実装

    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<char>> tiles;

    // 2次元配列tilesに読み込み
    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        tiles.push_back(row);
    }

    // ステージ全体の幅・高さを計算
    const int tileSize = 32;
    mStageRecs.clear();
    mStageWidth = (int)tiles[0].size() * tileSize;
    mStageHeight = (int)tiles.size() * tileSize;

    for (int y = 0; y < (int)tiles.size(); ++y)
    {
        int startX = -1;
        for (int x = 0; x < (int)tiles[y].size(); ++x)
        {
            // 床の場合
            if (tiles[y][x] == '#')
            {
                // 1が始まったら記録
                if (startX == -1) startX = x;
            }
            else
            {
                // 床じゃないならrectangleをつくる
                if (startX != -1)
                {
                    // 1が途切れたらRectangleに変換
                    Rectangle r;
                    r.x = (float)startX * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)(x - startX) * tileSize;
                    r.height = (float)tileSize;
                    mStageRecs.push_back(r);
                    startX = -1;
                }
                // 敵出現位置の場合
                if (tiles[y][x] == 'E')
                {
                    Vector2 pos = { (float)x * tileSize + tileSize / 2.0f, (float)y * tileSize };
                    mEnemySpawnPoints.push_back(pos);
                }
                else if (tiles[y][x] == 'W')
                {
                    StageObject* bo = new StageObject(mGamePlay);
                    bo->setPosition(Vector2{ (float)x * tileSize + tileSize / 2.0f,
                        (float)y * tileSize + tileSize / 2.0f });
                    bo->computeRectangle();
                }
                else if (tiles[y][x] == 'B')
                {
                    // アクターにしてもいい
                    BossEntrance.x = (float)x * tileSize;
                    BossEntrance.y = (float)y * tileSize;
                    BossEntrance.width = tileSize;
                    BossEntrance.height = tileSize;
                }
            }
        }
        // 行末まで1が続いていた場合
        if (startX != -1)
        {
            Rectangle r;
            r.x = (float)startX * tileSize;
            r.y = (float)y * tileSize;
            r.width = (float)(tiles[y].size() - startX) * tileSize;
            r.height = (float)tileSize;
            mStageRecs.push_back(r);
        }
    }
}

void Stage::update()
{
    // プレイヤーとスポーン地点の距離を計算して、近づいたら敵を出現させる
    PlayerActor* player = mGamePlay->getPlayer();

    Vector2 playerPos = player->getPosition();

    auto iter = mEnemySpawnPoints.begin();
    while (iter != mEnemySpawnPoints.end()) {
        float dist = Vector2Distance(playerPos, *iter);
        // スクリーンの半分以下の距離になったら出現にしてみた
        if (dist < GetScreenWidth() / 2.0f + 10.0f) {
            mSpawner->spawnAt(*iter);
            iter = mEnemySpawnPoints.erase(iter);
        }
        else {
            ++iter;
        }
    }

    // プレイヤーがボスエリアの入口に侵入したら
    if (CheckCollisionRecs(player->getRectangle(), BossEntrance)) {
        BossEntrance.width = 0.0f;
        BossEntrance.height = 0.0f;
        BossEntrance.x = 0.0f;
        BossEntrance.y = 0.0f;
        mGamePlay->onEnterBossArea();
    }
}

void Stage::draw()
{
    for (auto& rec : mStageRecs)
    {
        DrawRectangleRec(rec, GRAY);
    }
}
