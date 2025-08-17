#include "Title.h"
#include "GamePlay.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::input()
{
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new GamePlay(); // 次のシーケンスをGamePlayにセット
	}
}

void Title::update()
{
}

void Title::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawText("TITLE", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> GamePlay", 100, 200, 20, GRAY);

	EndDrawing();
}

Sequence* Title::nextSequence()
{
	return mNext;
}
