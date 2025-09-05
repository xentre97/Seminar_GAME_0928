#include "GamePlay.h"
#include "GameClear.h"
#include "GameOver.h"

#include <raylib.h>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

// Actor
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "WeaponActor.h"
// Component
#include "PlayerMove.h"
#include "EnemyMove.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"

#include "EnemySpawner.h"

GamePlay::GamePlay()
{
    // ステージファイルの読み込み
    if (!loadStage("stage0.txt")) {
        // ステージの読み込み失敗
    }
    // とりあえずプレイヤーとスポナーを生成
    mPlayer = new PlayerActor(this, Actor::Eplayer);
    mSpawner = new EnemySpawner(this);

    mSpawner->addSpawnPoint({ 700.0f, 100.0f });
    mSpawner->spawn();
}

GamePlay::~GamePlay()
{
    // deleteはSequenceやdestroyEnemyで対応するのでここでは削除しない
    /*while (!mEnemies.empty()) {
        delete mEnemies.back();
    }
    delete mPlayer;*/
    delete mSpawner;
    mSpawner = nullptr;
}

void GamePlay::input()
{
    // テスト入力
    if (IsKeyPressed(KEY_ENTER)) {
        mNext = new GameClear();
    }
    else if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        mNext = new GameOver();
    }
    // すべてのActorのinputを呼ぶ
    mUpdatingActors = true;
    for (Actor* actor : mActors) {
        actor->input();
    }
    mUpdatingActors = false;
}

void GamePlay::update()
{
    // Actorのupdate
    mUpdatingActors = true;
    for (Actor* actor : mActors) {
        actor->update();
    }
    mUpdatingActors = false;

    // 保留中のActorを追加
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // 衝突判定のupdate
    // 各Actor(およびComponent)のupdate後に行う
    updateCollision();

    // Dead状態のActorをdelete
    for (auto actor : mActors)
    {
        if (actor->getState() == Actor::Edead)
        {
            delete actor;
        }
    }
}

void GamePlay::draw()
{
    BeginDrawing();
    ClearBackground(WHITE);

    // uiの描画
    DrawText("GamePlay", 100, 100, 40, BLACK);
    DrawText("Press ENTER -> GameClear", 100, 200, 20, GRAY);
    DrawText("Press RightShift -> GameOver", 100, 300, 20, GRAY);

    // カメラに従って描画（ゲーム画面）
    BeginMode2D(mPlayer->getCamera());
    for (auto& rec : mStageRecs)
    {
        DrawRectangleRec(rec, GRAY);
    }
    for (auto sprite : mSprites)
    {
        sprite->draw();
    }
    EndMode2D();

    EndDrawing();
}

void GamePlay::unloadData()
{
    Sequence::unloadData();
    while (!mActors.empty())
    {
        delete mActors.back();
    }
}

Sequence* GamePlay::nextSequence()
{
    return mNext;
}

bool GamePlay::loadStage(const char* filename)
{
    // ステージファイルの読み込み
    // 0 : 空白（通行可能）
    // 1 : ブロック（壁）
    // 今はとりあえずシンプルな実装

    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<int>> tiles;

    // 2次元配列tilesに読み込み
    while (std::getline(file, line))
    {
        std::vector<int> row;
        for (char c : line)
        {
            if (c == '0') row.push_back(0);
            else if (c == '1') row.push_back(1);
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
            if (tiles[y][x] == 1)
            {
                // 1が始まったら記録
                if (startX == -1) startX = x;
            }
            else
            {
                if (startX != -1)
                {
                    // 1が途切れたらRectangleに変換
                    Rectangle r;
                    r.x = startX * tileSize;
                    r.y = y * tileSize;
                    r.width = (x - startX) * tileSize;
                    r.height = tileSize;
                    mStageRecs.push_back(r);
                    startX = -1;
                }
            }
        }
        // 行末まで1が続いていた場合
        if (startX != -1)
        {
            Rectangle r;
            r.x = startX * tileSize;
            r.y = y * tileSize;
            r.width = (tiles[y].size() - startX) * tileSize;
            r.height = tileSize;
            mStageRecs.push_back(r);
        }
    }
    return true;
}

void GamePlay::addEnemy(EnemyActor* enemy)
{
    mEnemies.emplace_back(enemy);
}

void GamePlay::removeEnemy(EnemyActor* enemy)
{
    // mEnemiesから削除
    auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
    if (iter != mEnemies.end()) {
        std::iter_swap(iter, mEnemies.end() - 1);
        mEnemies.pop_back();
    }
}

void GamePlay::addWeapon(WeaponActor* weapon, Actor::Type type)
{
    if (type == Actor::Eplayer) {
        mPlayerWeapons.emplace_back(weapon);
    }
    else if (type == Actor::Eenemy) {
        mEnemyWeapons.emplace_back(weapon);
    }
}

void GamePlay::removeWeapon(WeaponActor* weapon)
{
    auto iter = std::find(mPlayerWeapons.begin(), mPlayerWeapons.end(), weapon);
    if (iter != mPlayerWeapons.end()) {
        std::iter_swap(iter, mPlayerWeapons.end() - 1);
        mPlayerWeapons.pop_back();
    }
    else {
        iter = std::find(mEnemyWeapons.begin(), mEnemyWeapons.end(), weapon);
        if (iter != mEnemyWeapons.end()) {
            std::iter_swap(iter, mEnemyWeapons.end() - 1);
            mEnemyWeapons.pop_back();
        }
    }
}

void GamePlay::addSprite(SpriteComponent* sprite)
{
    // スプライトの描画順序を考慮して追加
    int myDrawOrder = sprite->getDrawOrder();
    auto iter = mSprites.begin();
    for (;
        iter != mSprites.end();
        ++iter)
    {
        if (myDrawOrder < (*iter)->getDrawOrder())
        {
            break;
        }
    }
    // 適切な位置に挿入
    mSprites.insert(iter, sprite);
}

void GamePlay::removeSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void GamePlay::updateCollision()
{
    Rectangle playerRec = mPlayer->getRectangle();

    // PlayerWeaponとEnemyの当たり判定
    for (auto enemy : mEnemies) {
        for (auto weapon : mPlayerWeapons)
        {
            Rectangle enemyRec = enemy->getRectangle();
            Rectangle weaponRec = weapon->getRectangle();
            if (CheckCollisionRecs(enemyRec, weaponRec)) {
                weapon->onHit(enemy);
            }
        }
    }

    // EnemyWeaponとPlayerの当たり判定
    for (auto weapon : mEnemyWeapons)
    {
        Rectangle playerRec = mPlayer->getRectangle();
        Rectangle weaponRec = weapon->getRectangle();
        if (CheckCollisionRecs(playerRec, weaponRec)) {
            // ダメージ処理
            mNext = new GameOver();
        }
    }

    // EnemyとPlayerの当たり判定
    for (auto enemy : mEnemies) {
        if (CheckCollisionRecs(playerRec, enemy->getRectangle())) {
            // ダメージ処理
            mNext = new GameOver();
        }
    }
    // PlayerがEnemyを倒したときにWeaponComponentから処理を通知する予定

    // Playerと地形の当たり判定
    for (auto& stageRec : mStageRecs) {
        // 当たり判定
        if (CheckCollisionRecs(playerRec, stageRec)) {
            // 衝突領域を取得
            Rectangle colRec = GetCollisionRec(playerRec, stageRec);
            Vector2 playerPos = mPlayer->getPosition();
            // 縦方向の衝突
            if (colRec.width >= colRec.height) {
                // 上から衝突
                if (playerRec.y < colRec.y) {
                    playerPos.y -= colRec.height;
                    //mPlayer->getPlayerMove().setJumping(false); // ジャンプ状態を解消
                    mPlayer->getPlayerMove()->fixFloorCol();
                }
                // 下から衝突
                else {
                    playerPos.y += colRec.height;
                }
            }
            // 横方向の衝突
            else {
                // 左から衝突
                if (playerRec.x < colRec.x) {
                    playerPos.x -= colRec.width;
                }
                // 右から衝突
                else {
                    playerPos.x += colRec.width;
                }
            }
            // 位置を更新
            mPlayer->setPosition(playerPos);
            // 矩形を再計算
            mPlayer->computeRectangle();
            // playerRecを更新
            playerRec = mPlayer->getRectangle();
        }
    }

    // Enemyと地形の当たり判定
    for (auto enemy : mEnemies) {
        Rectangle enemyRec = enemy->getRectangle();
        Vector2 enemyPos = enemy->getPosition();

        for (auto& stageRec : mStageRecs) {
            if (CheckCollisionRecs(enemyRec, stageRec)) {
                Rectangle colRec = GetCollisionRec(enemyRec, stageRec);
                // 横方向の衝突
                if (colRec.width < colRec.height) {
                    if (enemyRec.x < colRec.x) enemyPos.x -= colRec.width;
                    else enemyPos.x += colRec.width;

                    // 壁にぶつかったときの段差チェック（ジャンプさせる）
                    if (enemy->getEnemyState() != EnemyActor::E_jump)
                    {
                        const int tileSize = 32;
                        int forward = enemy->getForward();
                        bool isStep = false;
                        if (enemyPos.x < stageRec.x && forward > 0 ||
                            enemyPos.x > stageRec.x && forward < 0) {
                            // 段差が1.5タイル以内ならジャンプ
                            isStep = (stageRec.height <= tileSize * 1.5f);
                        }
                        // 段差の上にスペースがあるか確認
                        Rectangle checkOneAbove = {
                            colRec.x,
                            stageRec.y - tileSize,
                            colRec.width,
                            1.0f
                        };

                        bool isSpaceAboveClear = true;
                        for (const auto& otherStageRec : mStageRecs) {
                            if (CheckCollisionRecs(checkOneAbove, otherStageRec)) {
                                isSpaceAboveClear = false;
                                break;
                            }
                        }

                        if (isStep && isSpaceAboveClear) {
                            enemy->jump();
                            break;
                        }
                    }
                }
                // 縦方向の衝突
                else if (colRec.width >= colRec.height) {
                    // 上から衝突
                    if (enemyRec.y < colRec.y) {
                        enemyPos.y -= colRec.height;
                        enemy->getEnemyMove()->fixFloorCol();
                    }
                    // 下から衝突
                    else {
                        enemyPos.y += colRec.height;
                    }
                }
                // Enemyの位置更新
                enemy->setPosition(enemyPos);
                enemy->computeRectangle();
            }
        }
    }
}
