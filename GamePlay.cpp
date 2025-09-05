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

#include "EnemySpawner.h"

GamePlay::GamePlay()
{
	// TODO: 繝�く繧ｹ繝医ヵ繧｡繧､繝ｫ縺九ｉ繝槭ャ繝礼函謌
	if (!loadStage("stage0.txt")) {
		// 繝悶Ξ繝ｼ繧ｯ繝昴う繝ｳ繝育ｽｮ縺
	}
	// TODO:縺薙％縺ｧ繝槭ャ繝励�逕滓�,Player縺ｮ逕滓�遲芽｡後≧
	mPlayer = new PlayerActor(this, Actor::Eplayer);
	mSpawner = new EnemySpawner(this);

	mSpawner->addSpawnPoint({700.0f, 100.0f});
	mSpawner->spawn();}

GamePlay::~GamePlay()
{
	// 莉･荳九�delete縺ｯSequence繝�せ繝医Λ繧ｯ繧ｿ繧�estroyEnemy縺ｧ繧�▲縺ｦ繧九�縺ｧ荳榊ｿ�ｦ
	/*while (!mEnemies.empty()) {
		delete mEnemies.back();
	}
	delete mPlayer;*/
	delete mSpawner;
    mSpawner = nullptr;
}

void GamePlay::input()
{
	// test逕ｨ
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new GameClear();
	}
	else if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
		mNext = new GameOver();
	}
	// 蜈ｨActor縺ｮinput繧貞他縺ｶ
	mUpdatingActors = true;
	for (Actor* actor : mActors) {
		actor->input();
	}
	mUpdatingActors = false;
}

void GamePlay::update()
{
	// Actor縺ｮupdate
	mUpdatingActors = true;
	for (Actor* actor : mActors) {
		actor->update();
	}
	mUpdatingActors = false;
	// 菫晉蕗荳ｭ縺ｮActor繧知Actors縺ｸ遘ｻ蜍
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Collision縺ｮupdate
	// 蜈ｨ繧｢繧ｯ繧ｿ繝ｼ(縺ｨ縺昴�Component)縺ｮupdate蠕後↓蜻ｼ縺ｰ繧後※縺�ｋ縺薙→縺ｫ豕ｨ諢
	updateCollision();

	// Dead迥ｶ諷九�Actor繧壇elete
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

	// ui縺ｮ謠冗判
	DrawText("GamePlay", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> GameClear", 100, 200, 20, GRAY);
	DrawText("Press RightShift -> GameOver", 100, 300, 20, GRAY);

	// 繧ｫ繝｡繝ｩ縺ｫ遘ｻ縺吶ｂ縺ｮ縺ｮ謠冗判(ui莉･螟)
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
	// 繝�く繧ｹ繝医ヵ繧｡繧､繝ｫ縺九ｉ繧ｹ繝��繧ｸ繧定ｪｭ縺ｿ霎ｼ繧
	// 0 : 菴輔ｂ縺ｪ縺�ｴ謇
	// 1 : 蠎翫→螢
	// 邁｡譏鍋噪縺ｪ繧ゅ�縺ｧ縺
	// 1縺ｮ讓ｪ譁ｹ蜷代�騾｣邯壹ｒ荳縺､縺ｮrectangle縺ｨ縺励※隱崎ｭ倥＆縺帙※縺�ｋ

	std::ifstream file(filename);
	std::string line;
	std::vector<std::vector<int>> tiles;
	
	// 2谺｡蜈��蛻葉iles縺ｫ繝��繧ｿ繧堤ｧｻ縺
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
	// 荳縺､縺ｮ繧ｿ繧､繝ｫ縺ｮ邵ｦ讓ｪ縺ｮ髟ｷ縺
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
				// 1縺ｮ騾｣邯壹�蟾ｦ遶ｯ繧定ｨ倬鹸縺吶ｋ
				if (startX == -1) startX = x;
			}
			else
			{
				if (startX != -1)
				{
					// 讓ｪ譁ｹ蜷代�1縺ｮ騾｣邯壹ｒ縺ｾ縺ｨ繧√※Rectangle縺ｫ
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
		// 陦後�譛蠕後〒騾｣邯壹＠縺ｦ縺�◆蝣ｴ蜷
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
	// mEnemies縺九ｉ蜑企勁
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
	// 繧ｽ繝ｼ繝域ｸ医∩縺ｮ驟榊�縺ｧ謖ｿ蜈･轤ｹ繧定ｦ九▽縺代ｋ
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
	// 繧､繝�Ξ繝ｼ繧ｿ縺ｮ菴咲ｽｮ縺ｮ蜑阪↓隕∫ｴ繧呈諺蜈･縺吶ｋ
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

	// PlayerWeapon縺ｨEnemy縺ｮ陦晉ｪ∵､懃衍,蜃ｦ逅
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

	// TODO:EnemyWeapon縺ｨPlayer縺ｮ陦晉ｪ∵､懃衍,蜃ｦ逅
	for (auto weapon : mEnemyWeapons)
	{
		Rectangle playerRec = mPlayer->getRectangle();
		Rectangle weaponRec = weapon->getRectangle();
		if (CheckCollisionRecs(playerRec, weaponRec)) {
			// 蜊ｳ豁ｻ
			mNext = new GameOver();
		}
	}

	// TODO:謨ｵ縺ｨPlayer縺ｮ陦晉ｪ∵､懃衍,蜃ｦ逅
	for (auto enemy : mEnemies) {
		if (CheckCollisionRecs(playerRec, enemy->getRectangle())) {
			// 蜊ｳ豁ｻ
			mNext = new GameOver();
		}
	}
	// Player縺縺代〒縺ｪ縺拾nemy繧よ戟縺ｦ繧妓eaponComponent遲我ｽ懊ｌ縺ｰ濶ｯ縺�→諤昴≧

	// Player縺後�繝��縺ｨ陦晉ｪ√＠縺溘→縺阪�蜃ｦ逅
	for (auto& stageRec : mStageRecs) {
		// 陦晉ｪ√＠縺ｦ縺�ｋ縺ｪ繧
		if (CheckCollisionRecs(playerRec, stageRec)) {
			// 陦晉ｪ�Κ蛻��蝗幄ｧ貞ｽ｢繧貞ｾ励ｋ
			Rectangle colRec = GetCollisionRec(playerRec, stageRec);
			Vector2 playerPos = mPlayer->getPosition();
			// 邵ｦ譁ｹ蜷代�驥阪↑繧翫�譁ｹ縺悟ｰ上＆縺�ｴ蜷医�,邵ｦ縺ｮ驥阪↑繧翫□縺題ｧ｣豸
			if (colRec.width >= colRec.height) {
				// player菴咲ｽｮ繧剃ｸ翫↓縺壹ｉ縺
				if (playerRec.y < colRec.y) {
					playerPos.y -= colRec.height;
					//mPlayer->getPlayerMove().setJumping(false); // ジャンプ状態を解消
					mPlayer->getPlayerMove()->fixFloorCol();

				}
				// player菴咲ｽｮ繧剃ｸ九↓縺壹ｉ縺
				else {
					playerPos.y += colRec.height;
				}
			}
			// 讓ｪ譁ｹ蜷代�驥阪↑繧翫�譁ｹ縺悟ｰ上＆縺�ｴ蜷医�,讓ｪ縺ｮ驥阪↑繧翫□縺題ｧ｣豸
			else {
				// player菴咲ｽｮ繧貞ｷｦ縺ｫ縺壹ｉ縺
				if (playerRec.x < colRec.x) {
					playerPos.x -= colRec.width;
				}
				// player菴咲ｽｮ繧貞承縺ｫ縺壹ｉ縺
				else {
					playerPos.x += colRec.width;
				}
			}
			// 菴咲ｽｮ縺ｮ螟画峩繧貞渚譏
			mPlayer->setPosition(playerPos);
			// Player縺ｮ蝗幄ｧ貞ｽ｢繧ゅ★繧峨☆
			mPlayer->computeRectangle();
			// playerRec縺ｮ譖ｴ譁ｰ縺悟ｿ�ｦ
			playerRec = mPlayer->getRectangle();
		}
	}

	// Enemy縺後�繝��縺ｨ陦晉ｪ√＠縺溘→縺阪�蜃ｦ逅
	for (auto enemy : mEnemies) {
		Rectangle enemyRec = enemy->getRectangle();
		Vector2 enemyPos = enemy->getPosition();

		for (auto& stageRec : mStageRecs) {
			if (CheckCollisionRecs(enemyRec, stageRec)) {
				Rectangle colRec = GetCollisionRec(enemyRec, stageRec);
				//讓ｪ縺ｮ繧ｸ繝｣繝ｳ繝礼ｳｻ邨ｱ蜃ｦ逅
				if (colRec.width < colRec.height) {
					// 讓ｪ縺ｮ驥阪↑繧願ｧ｣豸
					if (enemyRec.x < colRec.x) enemyPos.x -= colRec.width;
					else enemyPos.x += colRec.width;
					// 繧ｸ繝｣繝ｳ繝嶺ｸｭ縺ｪ繧我ｻ･髯阪�蜃ｦ逅��荳崎ｦ
					if (enemy->getEnemyState() != EnemyActor::E_jump)
					{
						const int tileSize = 32;	
						int forward = enemy->getForward();
						bool isStep = false;
						if (enemyPos.x < stageRec.x && forward > 0 ||
							enemyPos.x > stageRec.x && forward < 0) {
							//騾ｲ陦梧婿蜷代�螢√′1繝槭せ谿ｵ蟾ｮ縺九メ繧ｧ繝�け
							isStep = (stageRec.height <= tileSize * 1.5f);
						}
						//�代�繧ｹ蜈医�ｼ代�繧ｹ荳翫′蟄伜惠縺吶ｋ縺九�遒ｺ隱阪�蝗幄ｧ貞ｽ｢螳夂ｾｩ
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
				//邵ｦ譁ｹ蜷代�驥阪↑繧翫′蟆上＆縺�ｴ蜷医∫ｸｦ縺ｮ驥阪↑繧願ｧ｣豸
				else if (colRec.width >= colRec.height) {
					//荳翫↓縺壹ｉ縺
					if (enemyRec.y < colRec.y) {
						enemyPos.y -= colRec.height;
						enemy->getEnemyMove()->fixFloorCol();
					}
					//荳九↓縺壹ｉ縺
					else {
						enemyPos.y += colRec.height;
					}
				}
				//enemy縺ｮ蝗幄ｧ貞ｽ｢繧貞�險育ｮ
				enemy->setPosition(enemyPos);
				enemy->computeRectangle();
			}
		}
	}
}
