#include "FelBall.h"
#include "Application.h"
#include "ModulePrinter.h"

#include "Scene.h"
#include "Guldan.h"
#include "ModuleInput.h"

FelBall::FelBall(const FelBallInfo& info, Projectile_type type) : Projectile(info, type)
{
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 21,33,14,14 });
//	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 69,32,18,23 });
//	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 119,32,18,23 });
//	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 168,32,18,23 });
//	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 217,32,18,23 });
//	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 266,32,18,23 });
//	felAnims[(uint)FelAnimations::moving_anim].speedFactor = 9.0f;

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

	switch (data.fel_movement)
	{
	case ProjectileInfo::fel_ball_movement::odd_even_type:

		timer += 1.0f * dt;

		if (timer <= TIME_ODD_EVEN)
		{
			toData->startRadius += toData->radiusToIncrease * dt;
			data.pos = RotateAround(data.pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}
		else
		{
			toData->startRadius += (toData->radiusToIncrease - RADIUS_DECREASE_ODD_EVEN) * dt;
			data.pos = RotateAround(data.pos, toData->rotationPivot, toData->angle * dt, toData->startRadius);
		}

		break;

	case ProjectileInfo::fel_ball_movement::complete_circle:
	case ProjectileInfo::fel_ball_movement::hexagon:

		toData->startRadius += toData->radiusToIncrease * dt;
		data.pos = RotateAround(data.pos, toData->rotationPivot, 0.0f, toData->startRadius);

		break;

	case ProjectileInfo::fel_ball_movement::spiral:

		if (App->scene->guldan->GetTimeToComeBackSpiral() < 5.0f)
		{
			toData->startRadius += toData->radiusToIncrease * dt;
			data.pos = RotateAround(data.pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}
		else
		{
			toData->startRadius -= 75.0f * dt;
			data.pos = RotateAround(data.pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}
		
		if (App->scene->guldan->pos.DistanceTo(data.pos) < 5.0f)
			App->projectiles->DestroyProjectile(this);

		break;
	}

	DecreaseLifePerTime(dt);

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool FelBall::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data.pos.x - 7,(int)data.pos.y - 7 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data.layer, ModulePrinter::Pivots::UPPER_LEFT);

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
