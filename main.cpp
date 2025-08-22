#include <raylib.h>
#include "Sequence.h"
#include "Title.h"



int main()
{
	/* Windowの初期化 */
	int WindowWidth = 960;
	int WindowHeight = 480;	
	InitWindow(WindowWidth, WindowHeight, "SeminarProject");
	SetTargetFPS(60); // 60FPS
	SetWindowState(FLAG_VSYNC_HINT); // 垂直同期ON

	// シーケンスを準備
	Sequence* currentSequence = new Title();

	// メインループ
	while (!WindowShouldClose())
	{
		/* 入力 */
		currentSequence->input();
		
		/* 更新 */
		currentSequence->update();

		/* 描画 */
		DrawFPS(10, 20); // FPSを表示 消してもいい
		currentSequence->draw();

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

	CloseWindow();
	return 0;
}
