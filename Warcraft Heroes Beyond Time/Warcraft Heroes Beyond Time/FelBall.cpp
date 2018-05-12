#include "FelBall.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "PlayerEntity.h"

#include "Scene.h"
#include "Guldan.h"
#include "ModuleInput.h"

#define BALL_DMG 10

FelBall::FelBall(const FelBallInfo* info, Projectile_type type) : Projectile(info, type)
{
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 21,33,14,14 });
	felAnims[(uint)FelAnimations::moving_anim].PushBack({ 36,33,14,14 });
	felAnims[(uint)FelAnimations::moving_anim].speedFactor = 4.5f;

	felAnims[(uint)FelAnimations::back_anim].PushBack({ 21,48,14,14 });
	felAnims[(uint)FelAnimations::back_anim].PushBack({ 36,48,14,14 });

	felAnims[(uint)FelAnimations::end_anim].PushBack({ 12,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].PushBack({ 42,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].PushBack({ 72,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].PushBack({ 102,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].PushBack({ 132,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].PushBack({ 162,407,28,28 });
	felAnims[(uint)FelAnimations::end_anim].speedFactor = 9.0f;
	felAnims[(uint)FelAnimations::end_anim].loop = false;
	//felAnims[(uint)FelAnimations::back_anim].speedFactor = 4.5f;

	actualAnim = &felAnims[(uint)FelAnimations::moving_anim];

	projCollider = *App->colliders->AddEnemyAttackCollider({ -7, -7, 14, 14 }, this, BALL_DMG, EnemyAttack::E_Attack_Type::GULDAN_BALL).lock();

	toData = (FelBallInfo*)info;
}

FelBall::~FelBall()
{
	App->colliders->deleteCollider(projCollider);
	RELEASE(data);
}

bool FelBall::Update(float dt)
{
	bool ret = true;

	switch (toData->fel_movement)
	{
	case FelBallInfo::fel_ball_movement::odd_even_type:

		timer += 1.0f * dt;

		if (timer <= TIME_ODD_EVEN)
		{
			toData->startRadius += toData->radiusToIncrease * dt;
			data->pos = RotateAround(data->pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}
		else
		{
			toData->startRadius += (toData->radiusToIncrease - RADIUS_DECREASE_ODD_EVEN) * dt;
			data->pos = RotateAround(data->pos, toData->rotationPivot, toData->angle * dt, toData->startRadius);
		}

		if (destroyTheBall)
		{
			actualAnim = &felAnims[(uint)FelAnimations::end_anim];
			toData->fel_movement = FelBallInfo::fel_ball_movement::explosion;
			App->colliders->deleteCollider(projCollider);
			break;
		}

		break;

	case FelBallInfo::fel_ball_movement::complete_circle:
	case FelBallInfo::fel_ball_movement::hexagon:

		toData->startRadius += toData->radiusToIncrease * dt;
		data->pos = RotateAround(data->pos, toData->rotationPivot, 0.0f, toData->startRadius);

		if (destroyTheBall)
		{
			actualAnim = &felAnims[(uint)FelAnimations::end_anim];
			toData->fel_movement = FelBallInfo::fel_ball_movement::explosion;
			App->colliders->deleteCollider(projCollider);
			break;
		}

		break;

	case FelBallInfo::fel_ball_movement::spiral:

		if (App->scene->guldan->GetTimeToComeBackSpiral() < TIME_UNTIL_BALLS_BACK_SPIRAL)
		{
			toData->startRadius += toData->radiusToIncrease * dt;
			data->pos = RotateAround(data->pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}
		else
		{
			actualAnim = &felAnims[(uint)FelAnimations::back_anim];
			toData->startRadius -= 75.0f * dt;
			data->pos = RotateAround(data->pos, toData->rotationPivot, 0.0f, toData->startRadius);
		}

		if (fPoint(App->scene->guldan->pos.x + 34, App->scene->guldan->pos.y + 34).DistanceTo(data->pos) < DISTANCE_TO_DELETE_FROM_GULDAN_SPIRAL)
			App->projectiles->DestroyProjectile(this);

		if (destroyTheBall)
		{
			actualAnim = &felAnims[(uint)FelAnimations::end_anim];
			toData->fel_movement = FelBallInfo::fel_ball_movement::explosion;
			App->colliders->deleteCollider(projCollider);
			break;
		}

		break;

	case FelBallInfo::fel_ball_movement::explosion:
		if (actualAnim->Finished())
			App->projectiles->DestroyProjectile(this);
		break;
	}

	if (slowSpeed)
	{
		if (toData->radiusToIncrease >= 10.0f && timeSlowed == 0.0f)
			toData->radiusToIncrease -= 10.0f;

		timeSlowed += 1.0f * dt;

		if (timeSlowed >= 2.0f)
		{
			slowSpeed = 0.0f;
			toData->radiusToIncrease += 10.0f;
			timeSlowed = 0.0f;
		}
	}

	DecreaseLifePerTime(dt);

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool FelBall::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - (actualAnim->GetCurrentRect().w / 2),(int)data->pos.y - (actualAnim->GetCurrentRect().h / 2) }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}

void FelBall::OnCollision(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
	case Collider::ColliderType::ENTITY:
	{
		Entity* entity = (Entity*)collideWith->owner;
		if (entity->entityType == Entity::EntityType::DYNAMIC_ENTITY)
		{
			DynamicEntity* dentity = (DynamicEntity*)entity;
			if (dentity->dynamicType == DynamicEntity::DynamicType::PLAYER)
			{
				PlayerEntity* pentity = (PlayerEntity*)dentity;
				if (pentity->state != PlayerEntity::states::PL_DASH)
					destroyTheBall = true;
			}
		}
		break;
	}		
	case Collider::ColliderType::WALL:
		if (data->life <= 999)
		{
			destroyTheBall = true;
		}
		break;

	case Collider::ColliderType::PLAYER_ATTACK:
		PlayerAttack* attack = (PlayerAttack*)collideWith;
		if (attack->pattacktype == PlayerAttack::P_Attack_Type::PROJECTILESLOWSHIT_ITEM)
		{
			int random = rand() % 100;
			if (random <= 10.0f)
				slowSpeed = true;
		}
	}
}

void FelBall::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
	
}

void FelBall::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
