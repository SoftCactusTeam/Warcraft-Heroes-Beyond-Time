#include "FelBall.h"

FelBall::FelBall(FelBallInfo& info) : data(info)
{
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 19,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 69,32,18,23 });

	actualAnim = &felAnims[(uint)FelAnimations::moving_anim];
}

FelBall::~FelBall()
{
}

bool FelBall::Update(float dt)
{
	bool ret = true;

	DecreaseLifePerTime(dt);

	return ret;
}

void FelBall::OnCollision(Collider* yours, Collider* collideWith)
{
}

void FelBall::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void FelBall::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
