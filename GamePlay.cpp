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
// Comopnent
#include "PlayerMove.h"
#include "EnemyMove.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"

GamePlay::GamePlay()
{
	// TODO: テキストファイルからマップ生成
	if (!loadStage("stage0.txt")) {
		// ブレークポイント置く
	}
	// TODO:ここでマップの生成,Playerの生成等行う
	mPlayer = new PlayerActor(this, Actor::Eplayer);
	new EnemyActor(this, Actor::Eenemy);
}

GamePlay::~GamePlay()
{
	// 以下のdeleteはSequenceデストラクタやdestroyEnemyでやってるので不必要
	/*while (!mEnemies.empty()) {
		delete mEnemies.back();
	}
	delete mPlayer;*/
}

void GamePlay::input()
{
	// test用
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new GameClear();
	}
	else if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
		mNext = new GameOver();
	}
	// 全Actorのinputを呼ぶ
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
	// 保留中のActorをmActorsへ移動
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Collisionのupdate
	// 全アクター(とそのComponent)のupdate後に呼ばれていることに注意
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

	// カメラに移すものの描画(ui以外)
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
	// テキストファイルからステージを読み込む
	// 0 : 何もない場所
	// 1 : 床と壁
	// 簡易的なものです
	// 1の横方向の連続を一つのrectangleとして認識させている

	std::ifstream file(filename);
	std::string line;
	std::vector<std::vector<int>> tiles;
	
	// 2次元配列tilesにデータを移す
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
	// 一つのタイルの縦横の長さ
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
				// 1の連続の左端を記録する
				if (startX == -1) startX = x;
			}
			else
			{
				if (startX != -1)
				{
					// 横方向の1の連続をまとめてRectangleに
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
		// 行の最後で連続していた場合
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
	// ソート済みの配列で挿入点を見つける
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
	// イテレータの位置の前に要素を挿入する
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

	// PlayerWeaponとEnemyの衝突検知,処理
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

	// TODO:EnemyWeaponとPlayerの衝突検知,処理
	for (auto weapon : mEnemyWeapons)
	{
		Rectangle playerRec = mPlayer->getRectangle();
		Rectangle weaponRec = weapon->getRectangle();
		if (CheckCollisionRecs(playerRec, weaponRec)) {
			// 即死
			mNext = new GameOver();
		}
	}

	// TODO:敵とPlayerの衝突検知,処理
	for (auto enemy : mEnemies) {
		if (CheckCollisionRecs(playerRec, enemy->getRectangle())) {
			// 即死
			mNext = new GameOver();
		}
	}
	// PlayerだけでなくEnemyも持てるWeaponComponent等作れば良いと思う

	// Playerがマップと衝突したときの処理
	for (auto& stageRec : mStageRecs) {
		// 衝突しているなら
		if (CheckCollisionRecs(playerRec, stageRec)) {
			// 衝突部分の四角形を得る
			Rectangle colRec = GetCollisionRec(playerRec, stageRec);
			Vector2 playerPos = mPlayer->getPosition();
			// 縦方向の重なりの方が小さい場合は,縦の重なりだけ解消
			if (colRec.width >= colRec.height) {
				// player位置を上にずらす
				if (playerRec.y < colRec.y) {
					playerPos.y -= colRec.height;
					//mPlayer->getPlayerMove().setJumping(false); // ジャンプ状態を解消
					mPlayer->getPlayerMove()->fixFloorCol();
				}
				// player位置を下にずらす
				else {
					playerPos.y += colRec.height;
				}
			}
			// 横方向の重なりの方が小さい場合は,横の重なりだけ解消
			else {
				// player位置を左にずらす
				if (playerRec.x < colRec.x) {
					playerPos.x -= colRec.width;
				}
				// player位置を右にずらす
				else {
					playerPos.x += colRec.width;
				}
			}
			// 位置の変更を反映
			mPlayer->setPosition(playerPos);
			// Playerの四角形もずらす
			mPlayer->computeRectangle();
			// playerRecの更新が必要
			playerRec = mPlayer->getRectangle();
		}
	}

	// Enemyがマップと衝突したときの処理
	for (auto enemy : mEnemies) {
		Rectangle enemyRec = enemy->getRectangle();
		Vector2 enemyPos = enemy->getPosition();

		for (auto& stageRec : mStageRecs) {
			if (CheckCollisionRecs(enemyRec, stageRec)) {
				Rectangle colRec = GetCollisionRec(enemyRec, stageRec);
				//横のジャンプ系統処理
				if (colRec.width < colRec.height) {
					// 横の重なり解消
					if (enemyRec.x < colRec.x) enemyPos.x -= colRec.width;
					else enemyPos.x += colRec.width;
					// ジャンプ中なら以降の処理は不要
					if (enemy->getEnemyState() != EnemyActor::E_jump)
					{
						const int tileSize = 32;	
						int forward = enemy->getForward();
						bool isStep = false;
						if (enemyPos.x < stageRec.x && forward > 0 ||
							enemyPos.x > stageRec.x && forward < 0) {
							//進行方向の壁が1マス段差かチェック
							isStep = (stageRec.height <= tileSize * 1.5f);
						}
						//１マス先、１マス上が存在するかの確認の四角形定義
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
				//縦方向の重なりが小さい場合、縦の重なり解消
				else if (colRec.width >= colRec.height) {
					//上にずらす
					if (enemyRec.y < colRec.y) {
						enemyPos.y -= colRec.height;
						enemy->getEnemyMove()->fixFloorCol();
					}
					//下にずらす
					else {
						enemyPos.y += colRec.height;
					}
				}
				//enemyの四角形を再計算
				enemy->setPosition(enemyPos);
				enemy->computeRectangle();
			}
		}
	}
}
