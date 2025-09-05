#pragma once

/// <summary>
/// UI�̊��
/// </summary>
class UIScreen
{
public:
	UIScreen(class Sequence* sequence);
	virtual ~UIScreen() {};

	virtual void update() {};
	virtual void draw() {};

	// ��ԊǗ�
	enum UIState
	{
		EActive,
		EClosing
	};
	void Close();
	UIState GetState() const { return mState; }

protected:
	class Sequence* mSequence;
	UIState mState;
};
