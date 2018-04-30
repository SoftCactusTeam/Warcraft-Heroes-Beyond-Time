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
	return ret;
}

bool ArcherArrow::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x,(int)data->pos.y}, (SDL_Texture*)App->projectiles->GetProjectileClassicAtlas(), { 808, 110, 32, 32 }, data->layer, ModulePrinter::Pivots::UPPER_LEFT);

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
