#include "Enemy_Archer.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "ModuleInput.h"
#include "ModuleMapGenerator.h"

#define DISTANCE_TO_MOVE	300
#define DISTANCE_TO_ATAC	70
#define ATAC_COOLDOWN		1000
#define MOVEMENT_SPEED		5

Enemy_Archer::Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Archer::Start()
{
	ChargeAnimations();
	state = ARCHER_STATE::ARCHER_IDLE;
	return true;
}

bool Enemy_Archer::Update(float dt)
{
	// AIXO ES PER COMPROBAR SI ESTA PARADA O NO
	if (stop == true)
		if (SDL_GetTicks() > accountantPrincipal)
			stop = false;
		else
			return true;

	switch (state)
	{
	case ARCHER_STATE::ARCHER_IDLE:
		doIdle();
		break;
	case ARCHER_STATE::ARCHER_WALK:
		doWalk();
		break;
	case ARCHER_STATE::ARCHER_ATAC:
		doAtac();
		break;
	default:
		state = ARCHER_STATE::ARCHER_IDLE;
		pathVector.Clear();
		break;
	}

	pathVector.PrintAstar();

	return true;
}

bool Enemy_Archer::Finish()
{
	return true;
}

// FUNCIONS D'ESTAT	

void Enemy_Archer::doIdle()
{
	anim = &animIdle[LookAtPlayer()];
	if (DistanceToPlayer() < DISTANCE_TO_MOVE) {
		state = ARCHER_STATE::ARCHER_WALK;
		pathVector.Clear();
	}
}

void Enemy_Archer::doWalk()
{
	anim = &animWalk[LookAtPlayer()];
	if (DistanceToPlayer() > DISTANCE_TO_MOVE)
	{
		state = ARCHER_STATE::ARCHER_IDLE;
		pathVector.Clear();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_ATAC)
	{
		state = ARCHER_STATE::ARCHER_ATAC;
		accountantPrincipal = SDL_GetTicks() + ATAC_COOLDOWN;
		anim = &animAtac[LookAtPlayer()];
		anim->Reset();
		App->colliders->AddTemporalCollider({ (int)pos.x, (int)pos.y, 64, 64 }, COLLIDER_TYPE::COLLIDER_ENEMY_ATAC, ATAC_COOLDOWN);
	}
	else // AQUI CAMINA, PERO AQUESTA FUNCIO ES TEMPORAL
	{
		if (pathVector.isEmpty())
		{
			pathVector.CalculatePathAstar(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->entities->player->pos.x, (int)App->entities->player->pos.y));
			pathVector.CalculateWay(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->entities->player->pos.x, (int)App->entities->player->pos.y));
		}
		else
		{
			iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x, (int)pos.y));
			this->pos += fPoint((float)move.x * MOVEMENT_SPEED, (float)move.y * MOVEMENT_SPEED);
		}
	}
}

void Enemy_Archer::doAtac()
{
	if (SDL_GetTicks() > accountantPrincipal)
	{
		state = ARCHER_STATE::ARCHER_IDLE;
		pathVector.Clear();
	}
}

void Enemy_Archer::ChargeAnimations()
{

}