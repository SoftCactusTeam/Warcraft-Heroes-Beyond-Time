#include "ArcherArrow.h"
#include "Application.h"
#include "ModulePrinter.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "ModuleEntitySystem.h"

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
	toData->arrowCollider = App->colliders->AddCollider({ 0,0,8,8 }, Collider::ColliderType::ENEMY_ATTACK);
	toData->layer = 2;
	toData->deadTimer += SDL_GetTicks();	
}

ArcherArrow::~ArcherArrow()
{
	RELEASE(data);
}

bool ArcherArrow::Update(float dt)
{
	bool ret = true;

	//toData->arrowCollider->rectArea.x = (int)toData->pos.x;
	//toData->arrowCollider->rectArea.y = (int)toData->pos.y;

	std::list<Collider*>::iterator collidingWith = toData->arrowCollider->colliding.begin();
	for (; collidingWith != toData->arrowCollider->colliding.end(); collidingWith++)
	{
		switch ((*collidingWith)->colType)
		{
		case Collider::ColliderType::WALL:
			toData->tempoAtWall = 1000 + SDL_GetTicks();
			break;
		case Collider::ColliderType::ENTITY:
			Entity* entOwner = (Entity*)(*collidingWith)->owner;
			if (entOwner->entityType == Entity::EntityType::DYNAMIC_ENTITY)
			{
				DynamicEntity* dynOwner = (DynamicEntity*)(*collidingWith)->owner;
				if (dynOwner->dynamicType == DynamicEntity::DynamicType::PLAYER)
					App->projectiles->DestroyProjectile(this);
			}
			break;
		}
	}

	if (toData->tempoAtWall != -1)
	{
		if (toData->tempoAtWall < SDL_GetTicks())
			App->projectiles->DestroyProjectile(this);
	}
	else if (SDL_GetTicks() < toData->deadTimer)
	{
		fPoint copy = toData->direction;
		toData->pos += copy * toData->speed;
	}
	else
		App->projectiles->DestroyProjectile(this);
	return ret;
}

bool ArcherArrow::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)toData->pos.x,(int)toData->pos.y }, (SDL_Texture*)App->projectiles->GetProjectileClassicAtlas(), { 808, 110, 32, 32 }, toData->layer, ModulePrinter::Pivots::CENTER, { 0,0 }, ModulePrinter::Pivots::UPPER_LEFT, {0,0}, toData->angle);

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
