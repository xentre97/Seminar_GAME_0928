#pragma once
#include "Actor.h"

/// <summary>
/// PlayerActorƒNƒ‰ƒX
/// </summary>
class PlayerActor :
    public Actor
{
public:
    PlayerActor(class Sequence* sequence);

    void input() override;
    void update() override;
    void draw() override;

    const Camera2D& getCamera() const ;
    class PlayerMove& getPlayerMove();
    void computeRectangle();

private:
    class CameraComponent* mCameraComp;
    class PlayerMove* mPlayerMove;
};

