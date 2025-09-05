#include "UIScreen.h"
#include "Sequence.h"
#include <raylib.h>

UIScreen::UIScreen(Sequence* sequence)
	: mSequence(sequence)
	, mState(EActive)
{
	// UI�X�^�b�N��UI��ǉ�
	mSequence->pushUI(this);
}

void UIScreen::Close()
{
	mState = EClosing;
}
