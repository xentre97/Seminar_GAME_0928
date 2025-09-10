#include <raylib.h>
#include "Sequence.h"
#include "Title.h"


const int WindowWidth = 960;
const int WindowHeight = 480;

int main()
{
	/* Window�̏����� */
	InitWindow(WindowWidth, WindowHeight, "SeminarProject");
	SetTargetFPS(60); // 60FPS
	SetWindowState(FLAG_VSYNC_HINT); // ��������ON

	// �V�[�P���X������
	Sequence* currentSequence = new Title();

	// ���C�����[�v
	while (!WindowShouldClose())
	{
		/* ���� */
		currentSequence->input();
		
		/* �X�V */
		currentSequence->update();

		/* �`�� */
		currentSequence->draw();
		DrawFPS(10, 20); // FPS��\�� �����Ă�����
	
		// �V�[�P���X�؂�ւ�
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
