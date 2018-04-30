#include "ArcherArrow.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"

#include "Scene.h"
#include "ModuleInput.h"

ArcherArrow::ArcherArrow(const ArcherArrowInfo* info, Projectile_type type) : Projectile(info, type)
{
	arrowAnims[(uint)ArrowAnimations::moving_anim].PushBack({ 21,33,14,14 });
	arrowAnims[(uint)ArrowAnimations::moving_anim].PushBack({ 36,33,14,14 });
	arrowAnims[(uint)ArrowAnimations::moving_anim].speedFactor = 4.5f;

	actualAnim = &arrowAnims[(uint)ArrowAnimations::moving_anim];

	toData = (ArcherArrowInfo*)info;

	toData->angle = atan2(toData->pos.y - App->scene->player->pos.y, toData->pos.x - App->scene->player->pos.x);
	if (toData->angle > 0)
		toData->angle = toData->angle * 360 / (2 * PI);
	else
		toData->angle = (2 * PI + toData->angle) * 360 / (2 * PI);
	toData->angle -= 90;

	toData->tempoAtWall = -1;

	App->colliders->AddCollider({ 0,0,32,32 }, Collider::ColliderType::ENEMY_ATTACK);
}

ArcherArrow::~ArcherArrow()
{
	RELEASE(data);
}

bool ArcherArrow::Update(float dt)
{
	bool ret = true;

	if (toData->tempoAtWall != -1)
	{
		if (toData->tempoAtWall < SDL_GetTicks())
			App->projectiles->DestroyProjectile(this);
	}
	else if (SDL_GetTicks() < toData->deadTimer)
	{
		toData->pos += toData->direction;
	}
	else
		App->projectiles->DestroyProjectile(this);

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;

	/*if (stop == true)
		if (SDL_GetTicks() > accountantPrincipal)
			stop = false;
		else
		{
			for (int i = 0; i < arrowsVector.size(); i++)
				arrowsVector[i]->deadTimer += App->dt * 1000;
			return true;
		}
	else if (App->scene->paused == true)
	{
		for (int i = 0; i < arrowsVector.size(); i++)
			arrowsVector[i]->deadTimer += App->dt * 1000;
		return true;
	}

	for (int i = 0; i < arrowsVector.size(); i++)
	{
		if (arrowsVector[i]->destroy == false)
			arrowsVector[i]->Update();
		else
		{
			arrowsVector[i]->Finish();
			delete arrowsVector[i];
			arrowsVector.erase(arrowsVector.begin() + i);
		}
	}*/
}

bool ArcherArrow::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 7,(int)data->pos.y - 7 }, (SDL_Texture*)App->projectiles->GetProjectileClassicAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}

void ArcherArrow::OnCollision(Collider* yours, Collider* collideWith)
{
}

void ArcherArrow::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void ArcherArrow::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
