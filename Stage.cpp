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
    // �X�e�[�W�t�@�C���̓ǂݍ���
    // ' ' : ��(�ʍs�\)
    // # : �u���b�N(��)
    // w : �؂̔�
    // E : �G�̏o���ʒu
    // B : �{�X�G���A�̓��� �����Ƀv���C���[���ڐG������{�X�X�e�[�W�̓ǂݍ��݂��n�߂�
    // ���͂Ƃ肠�����V���v���Ȏ���

    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<char>> tiles;

    // 2�����z��tiles�ɓǂݍ���
    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        tiles.push_back(row);
    }

    // �X�e�[�W�S�̂̕��E�������v�Z
    const int tileSize = 32;
    mStageRecs.clear();
    mStageWidth = (int)tiles[0].size() * tileSize;
    mStageHeight = (int)tiles.size() * tileSize;

    for (int y = 0; y < (int)tiles.size(); ++y)
    {
        int startX = -1;
        for (int x = 0; x < (int)tiles[y].size(); ++x)
        {
            // ���̏ꍇ
            if (tiles[y][x] == '#')
            {
                // 1���n�܂�����L�^
                if (startX == -1) startX = x;
            }
            else
            {
                // ������Ȃ��Ȃ�rectangle������
                if (startX != -1)
                {
                    // 1���r�؂ꂽ��Rectangle�ɕϊ�
                    Rectangle r;
                    r.x = (float)startX * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)(x - startX) * tileSize;
                    r.height = (float)tileSize;
                    mStageRecs.push_back(r);
                    startX = -1;
                }
                // �G�o���ʒu�̏ꍇ
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
                    // �A�N�^�[�ɂ��Ă�����
                    BossEntrance.x = (float)x * tileSize;
                    BossEntrance.y = (float)y * tileSize;
                    BossEntrance.width = tileSize;
                    BossEntrance.height = tileSize;
                }
            }
        }
        // �s���܂�1�������Ă����ꍇ
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
    // �v���C���[�ƃX�|�[���n�_�̋������v�Z���āA�߂Â�����G���o��������
    PlayerActor* player = mGamePlay->getPlayer();

    Vector2 playerPos = player->getPosition();

    auto iter = mEnemySpawnPoints.begin();
    while (iter != mEnemySpawnPoints.end()) {
        float dist = Vector2Distance(playerPos, *iter);
        // �X�N���[���̔����ȉ��̋����ɂȂ�����o���ɂ��Ă݂�
        if (dist < GetScreenWidth() / 2.0f + 10.0f) {
            mSpawner->spawnAt(*iter);
            iter = mEnemySpawnPoints.erase(iter);
        }
        else {
            ++iter;
        }
    }

    // �v���C���[���{�X�G���A�̓����ɐN��������
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
