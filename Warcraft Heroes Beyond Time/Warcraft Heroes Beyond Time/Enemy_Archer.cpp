#include "Enemy_Archer.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "ModuleInput.h"
#include "ModuleMapGenerator.h"
#include "Scene.h"

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
			pathVector.CalculatePathAstar(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
			pathVector.CalculateWay(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
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

	// ANIM IDLE

	animIdle[FIXED_ANGLE::UP].PushBack({ 153,179,42,48 });
	animIdle[FIXED_ANGLE::UP].speed = 0.1f;

	animIdle[FIXED_ANGLE::UP_RIGHT].PushBack({ 101,225,44,47 });
	animIdle[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;

	animIdle[FIXED_ANGLE::RIGHT].PushBack({ 165,565,40,41 });
	animIdle[FIXED_ANGLE::RIGHT].speed = 0.1f;

	animIdle[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,371,44,39 });
	animIdle[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;

	animIdle[FIXED_ANGLE::DOWN].PushBack({ 1,241,46,44 });
	animIdle[FIXED_ANGLE::DOWN].speed = 0.1f;

	animIdle[FIXED_ANGLE::DOWN_LEFT].PushBack({ 142,398,44,39 });
	animIdle[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;

	animIdle[FIXED_ANGLE::LEFT].PushBack({ 90,530,40,42 });
	animIdle[FIXED_ANGLE::LEFT].speed = 0.1f;

	animIdle[FIXED_ANGLE::UP_LEFT].PushBack({ 150,269,44,47 });
	animIdle[FIXED_ANGLE::UP_LEFT].speed = 0.1f;

	// ANIM WALK

	animWalk[FIXED_ANGLE::UP].PushBack({ 153,179,42,48 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 157,1,44,51 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 109,89,43,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 160,94,38,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 153,134,43,48 });
	animWalk[FIXED_ANGLE::UP].speed = 0.1f;

	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 101,225,44,47 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,453,40,43 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 49,275,42,46 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,147,43,48 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 97,316,42,45 });
	animWalk[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;

	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 165,565,40,41 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 1,412,42,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 140,439,41,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 81,572,37,40 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 211,47,37,43 });
	animWalk[FIXED_ANGLE::RIGHT].speed = 0.1f;

	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,371,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 207,530,40,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 182,488,42,40 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,331,45,38 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 191,361,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;

	animWalk[FIXED_ANGLE::DOWN].PushBack({ 1,241,46,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 203,183,45,42 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 203,197,45,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 185,446,40,43 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 47,408,43,43 });
	animWalk[FIXED_ANGLE::DOWN].speed = 0.1f;

	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 142,398,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 207,571,40,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 136,523,42,40 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 144,358,45,38 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 94,405,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;

	animWalk[FIXED_ANGLE::LEFT].PushBack({ 90,530,40,42 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 188,402,42,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 92,446,41,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 123,572,37,40 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 211,92,37,43 });
	animWalk[FIXED_ANGLE::LEFT].speed = 0.1f;

	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 150,269,44,47 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,456,40,43 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,287,42,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 98,271,43,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 49,319,42,45 });
	animWalk[FIXED_ANGLE::UP_LEFT].speed = 0.1f;

	//	ANIM ATAC

	animAtac[FIXED_ANGLE::UP].PushBack({ 1,1,49,54 });
	animAtac[FIXED_ANGLE::UP].PushBack({ 57,42,46,50 });
	animAtac[FIXED_ANGLE::UP].speed = 0.1f;

	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 109,42,45,49 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 101,183,40,48 });
	animAtac[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;

	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 151,223,47,44 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 46,495,41,42 });
	animAtac[FIXED_ANGLE::RIGHT].speed = 0.1f;

	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,572,38,38 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 167,482,43,39 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;

	animAtac[FIXED_ANGLE::DOWN].PushBack({ 199,273,47,38 });
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 146,315,46,41 });
	animAtac[FIXED_ANGLE::DOWN].speed = 0.1f;

	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 41,572,38,38 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 91,489,43,39 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;

	animAtac[FIXED_ANGLE::LEFT].PushBack({ 200,227,47,44 });
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 1,498,41,42 });
	animAtac[FIXED_ANGLE::LEFT].speed = 0.1f;

	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 160,47,45,49 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 51,185,40,48 });
	animAtac[FIXED_ANGLE::UP_LEFT].speed = 0.1f;

	//	ANIM DEATH

	animDeath[FIXED_ANGLE::UP].PushBack({ 1,98,47,48 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 57,1,53,39 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 96,360,44,43 });
	animDeath[FIXED_ANGLE::UP].speed = 0.1f;

	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,98,47,48 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 57,1,53,39 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 96,360,44,43 });
	animDeath[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;

	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 1,52,44,50 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 199,313,46,46 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 1,192,47,47 });
	animDeath[FIXED_ANGLE::RIGHT].speed = 0.1f;

	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,52,44,50 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 199,313,46,46 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,192,47,47 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;

	animDeath[FIXED_ANGLE::LEFT].PushBack({ 51,139,44,48 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 50,227,46,46 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 53,90,48,47 });
	animDeath[FIXED_ANGLE::LEFT].speed = 0.1f;

	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 51,139,44,48 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 50,227,46,46 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 53,90,48,47 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;

	animDeath[FIXED_ANGLE::DOWN].PushBack({ 103,134,47,48 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 112,1,53,39 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 48,363,44,43 });
	animDeath[FIXED_ANGLE::DOWN].speed = 0.1f;

	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 103,134,47,48 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 112,1,53,39 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 48,363,44,43 });
	animDeath[FIXED_ANGLE::UP_LEFT].speed = 0.1f;

}