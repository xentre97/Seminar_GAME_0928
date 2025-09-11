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
#include "StageObject.h"
// Component
#include "PlayerMove.h"
#include "EnemyMove.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"
#include "HpComponent.h"
// system
#include "Stage.h"
#include "CameraSystem.h"
#include "UIScreen.h"
#include "HUD.h"
#include "DamageUI.h"
#include "SoundSystem.h"

GamePlay::GamePlay()
{
    mStage = new Stage(this);
    // ステージファイルの読み込み
    mStage->loadStage("Assets/stage0.txt");

    // とりあえずプレイヤーとスポナーを生成
    mPlayer = new PlayerActor(this);

    // カメラシステムの初期化
    mCameraSystem = new CameraSystem((float)mStage->getStageWidth());
    mCameraSystem->setPlayer(mPlayer);
    mCameraSystem->setMode(CameraSystem::Mode::FollowPlayer);
    mHUD = new HUD(this);
    mDamageUI = new DamageUI(this);

    // サウンドの読み込み
    SoundSystem &s = SoundSystem::instance();
    s.loadBGM("stageBGM", "Assets/uchinari_138.mp3");
    s.playBGM("stageBGM");
}

GamePlay::~GamePlay()
{
    // deleteはSequenceやdestroyEnemyで対応するのでここでは削除しない
    /*while (!mEnemies.empty()) {
        delete mEnemies.back();
    }
    delete mPlayer;*/
    delete mHUD;
    mHUD = nullptr;

    delete mCameraSystem;
    mCameraSystem = nullptr;
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
    mCameraSystem->update();
    mStage->update();

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

    // Dead状態のActorをdelete
    for (auto actor : mActors)
    {
        if (actor->getState() == Actor::Edead)
        {
            delete actor;
        }
    }

    // ui update
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::EActive) {
            ui->update();
        }
    }
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::EClosing) {
            delete* iter;
            iter = mUIStack.erase(iter);
        }
        else {
            ++iter;
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
    BeginMode2D(mCameraSystem->getCamera());
    
    mStage->draw();

    for (auto sprite : mSprites)
    {
        sprite->draw();
    }
    EndMode2D();

    // uiの描画
    for (auto ui : mUIStack) {
        ui->draw();
    }

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

void GamePlay::addStageObj(StageObject* enemy)
{
    mObjects.emplace_back(enemy);
}

void GamePlay::removeStageObj(StageObject* enemy)
{
    auto iter = std::find(mObjects.begin(), mObjects.end(), enemy);
    if (iter != mObjects.end()) {
        std::iter_swap(iter, mObjects.end() - 1);
        mObjects.pop_back();
    }
}

Camera2D GamePlay::getCamera() const
{
    if (mCameraSystem) return mCameraSystem->getCamera();
    else return Camera2D();
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

void GamePlay::onEnterBossArea()
{
    // 敵全削除
    while (!mEnemies.empty()) {
        delete mEnemies.back();
    }
    // ステージオブジェクト全削除
    while (!mObjects.empty()) {
        delete mObjects.back();
    }

    mStage->loadStage("Assets/stage0_boss.txt");
    // ステージから位置を拾ってきてもいいかも
    mPlayer->setPosition(Vector2{ 64.0f, 384.0f });
    mPlayer->computeRectangle();
    mCameraSystem->setMode(CameraSystem::Mode::Fixed);
    //mCameraSystem->setFixedTarget(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f });
}
