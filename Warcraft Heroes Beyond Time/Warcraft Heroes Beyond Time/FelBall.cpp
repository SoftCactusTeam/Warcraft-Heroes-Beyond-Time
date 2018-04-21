#include "FelBall.h"
#include "Application.h"
#include "ModulePrinter.h"

#include <math.h>

FelBall::FelBall(const FelBallInfo& info, Projectile_type type) : Projectile(info, type)
{
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 19,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 69,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 119,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 168,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 217,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 266,32,18,23 });
	felAnims[(uint)FelAnimations::moving_anim].speedFactor = 9.0f;

	actualAnim = &felAnims[(uint)FelAnimations::moving_anim];

	toData = &(FelBallInfo&)data;
	toData->rotationPivot = info.rotationPivot;
}

FelBall::~FelBall()
{
}

bool FelBall::Update(float dt)
{
	bool ret = true;

	int toNormX = toData->rotationPivot.x - (int)toData->pos.x;
	int toNormY = toData->rotationPivot.y - (int)toData->pos.y;
	int norm = sqrt(pow(toNormX, 2) + pow(toNormY, 2));

	angle = atan2(toNormY/norm, toNormX/norm );

	DecreaseLifePerTime(dt);

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool FelBall::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data.pos.x,(int)data.pos.y }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data.layer, ModulePrinter::Pivots::UPPER_LEFT, angle);

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
