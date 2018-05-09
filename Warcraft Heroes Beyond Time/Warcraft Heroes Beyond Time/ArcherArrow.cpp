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

	toData->angle = atan2(toData->pos.y - info->initialPlayerPos.y, toData->pos.x - info->initialPlayerPos.x);
	if (toData->angle > 0)
		toData->angle = toData->angle * 360 / (2 * PI);
	else
		toData->angle = (2 * PI + toData->angle) * 360 / (2 * PI);
	toData->angle -= 90;

	toData->tempoAtWall = -1;

	toData->arrowCollider = *App->colliders->AddEnemyAttackCollider({ 0,0,8,8 }, this, info->damageArrow, EnemyAttack::E_Attack_Type::ARROW).lock();
	toData->layer = 2;
	toData->deadTimer += SDL_GetTicks();
	deleteArrow = false;
	toData->copySpeed = toData->speed;

}

ArcherArrow::~ArcherArrow()
{
	App->colliders->deleteColliderbyOwner(this);
	RELEASE(data);
}

bool ArcherArrow::Update(float dt)
{
	bool ret = true;

	if (deleteArrow == true)
		App->projectiles->DestroyProjectile(this);

	if (toData->arrowCollider != nullptr)
	{
		toData->arrowCollider->rectArea.x = (int)toData->pos.x  + toData->direction.x * 12 /*16 - 4*/;
		toData->arrowCollider->rectArea.y = (int)toData->pos.y  + toData->direction.y * 12;
	}
	

	if (toData->tempoAtWall != -1)
	{
		App->colliders->deleteCollider(toData->arrowCollider);
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

	ret = App->printer->PrintSprite({ (int)toData->pos.x,(int)toData->pos.y }, (SDL_Texture*)App->projectiles->GetProjectileClassicAtlas(), { 808, 110, 32, 32 }, toData->layer, ModulePrinter::Pivots::CENTER, { 0,0 }, ModulePrinter::Pivots::CENTER, {0,0}, toData->angle);

	return ret;
}

void ArcherArrow::OnCollision(Collider* yours, Collider* collideWith)
{
	printf_s("%i\n", (int)collideWith->colType);
	switch (collideWith->colType)
	{

	case Collider::ColliderType::WALL:
		toData->tempoAtWall = 1000 + SDL_GetTicks();
		break;

	case Collider::ColliderType::ENTITY:
	{
		Entity* entOwner = (Entity*)collideWith->owner;
		if (entOwner->entityType == Entity::EntityType::DYNAMIC_ENTITY)
		{
			DynamicEntity* dynOwner = (DynamicEntity*)collideWith->owner;
			if (dynOwner->dynamicType == DynamicEntity::DynamicType::PLAYER)
			{
				PlayerEntity* plaOwner = (PlayerEntity*)collideWith->owner;
				if (plaOwner->GetDamageCollider() == collideWith)
					if (!plaOwner->getConcretePlayerStates(11))	// el num 11 es el dash, es una guarrada per mira ... no tinc temps ...
					// AIXO SI L'ALTRE ES UN PLAYER
						deleteArrow = true;
			}
		}
	}
	break;

	case Collider::ColliderType::PLAYER_ATTACK:
	{
		PlayerAttack* attack = (PlayerAttack*)collideWith;
		switch (attack->pattacktype)
		{
		case PlayerAttack::P_Attack_Type::NORMAL_ATTACK:
			deleteArrow = true;
			break;
		case PlayerAttack::P_Attack_Type::DAMAGESHIT_ITEM:
			toData->speed = 2;
			break;
		case PlayerAttack::P_Attack_Type::FREEZEBALL_ITEM:
			deleteArrow = true;
			break;
		}
	}
	}
}

void ArcherArrow::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void ArcherArrow::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
	if (collideWith->colType == Collider::ColliderType::PLAYER_ATTACK)
	{
		PlayerAttack* attack = (PlayerAttack*)collideWith;
		switch (attack->pattacktype)
		{
		case PlayerAttack::P_Attack_Type::DAMAGESHIT_ITEM:
			toData->speed = toData->copySpeed;
			break;
		}
	}
}
