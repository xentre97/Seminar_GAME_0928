#include <raylib.h>
#include "Sequence.h"
#include "Title.h"
#include "SoundSystem.h"

const int WindowWidth = 960;
const int WindowHeight = 480;

int main()
{
	/* Windowの初期化 */
	InitWindow(WindowWidth, WindowHeight, "SeminarProject");
	SetTargetFPS(60); // 60FPS
	SetWindowState(FLAG_VSYNC_HINT); // 垂直同期ON

	// サウンドシステム初期化
	SoundSystem::init();

	// シーケンスを準備
	Sequence* currentSequence = new Title();

	// メインループ
	while (!WindowShouldClose())
	{
		/* 入力 */
		currentSequence->input();
		
		/* 更新 */
		currentSequence->update();

		/* サウンド更新 */
		SoundSystem::instance().update();

		/* 描画 */
		currentSequence->draw();
		DrawFPS(10, 20); // FPSを表示 消してもいい
	
		// シーケンス切り替え
		Sequence* next = currentSequence->nextSequence();
		if (next != nullptr) {
			currentSequence->unloadData();
			delete currentSequence;
			currentSequence = next;
		}
	}
	
	currentSequence->unloadData();
	delete currentSequence;
	currentSequence = 0;

	SoundSystem::shutdown();

	CloseWindow();
	return 0;
}
