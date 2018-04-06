#include "Enemy_Footman.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "ModuleInput.h"
#include "ModuleMapGenerator.h"
#include "Scene.h"

#define DISTANCE_TO_MOVE	300
#define DISTANCE_TO_CHARGE	150
#define DISTANCE_TO_ATAC	40
#define CHARGE_DISTANCE		50
#define CHARGE_SPEED		5
#define CHARGE_COOLDOWN		3000
#define CHARGE_PRETIME		1000
#define ATAC_COOLDOWN		1000
#define MOVEMENT_SPEED		3
#define DEFENSE_DISTANCE	130
#define DEFENSE_TIME		1000
#define DEFENSE_COOLDOWN	3000

Enemy_Footman::Enemy_Footman(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Footman::Start()
{
	ChargeAnimations();
	state = FOOTMAN_STATE::FOOTMAN_IDLE;
	return true;
}

bool Enemy_Footman::Update(float dt)
{
	// AIXO ES PER COMPROBAR SI ESTA PARADA O NO
	if (stop == true)
		if (SDL_GetTicks() > accountantPrincipal)
			stop = false;
		else
			return true;

	switch (state)
	{
	case FOOTMAN_STATE::FOOTMAN_IDLE:
		doIdle();
		break;
	case FOOTMAN_STATE::FOOTMAN_WALK:
		doWalk();
		break;
	case FOOTMAN_STATE::FOOTMAN_ATAC:
		doAtac();
		break;
	case FOOTMAN_STATE::FOOTMAN_CHARGE:
		doCharge();
		break;
	default:
		initIdle();
		break;
	}
	pathVector.PrintAstar();
	return true;
}

bool Enemy_Footman::Finish()
{
	return true;
}

// FUNCIONS D'ESTAT	
void Enemy_Footman::initIdle()
{
	state = FOOTMAN_STATE::FOOTMAN_IDLE;
	pathVector.Clear();
}

void Enemy_Footman::initWalk()
{
	state = FOOTMAN_STATE::FOOTMAN_WALK;
	pathVector.Clear();
}

void Enemy_Footman::initAtac()
{
	state = FOOTMAN_STATE::FOOTMAN_ATAC;
	accountantPrincipal = SDL_GetTicks() + ATAC_COOLDOWN;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	App->colliders->AddTemporalCollider({ (int)pos.x, (int)pos.y, 64, 64 }, COLLIDER_TYPE::COLLIDER_ENEMY_ATAC, ATAC_COOLDOWN);
}

void Enemy_Footman::initCharge()
{
	state = FOOTMAN_STATE::FOOTMAN_CHARGE;
	chargeTime = CHARGE_DISTANCE;
	anim = &animPreCharge[LookAtPlayer()];
	saveFirstAngle = LookAtPlayer();
	anim->Reset();
	chargeMovement = CaculateFPointAngle(App->scene->player->pos) * CHARGE_SPEED;
	chargeCooldown = SDL_GetTicks() + CHARGE_COOLDOWN;
	StopConcreteTime(CHARGE_PRETIME);
}

void Enemy_Footman::initDefense()
{
	state = FOOTMAN_STATE::FOOTMAN_DEFENSE;
	accountantPrincipal = DEFENSE_TIME;
	anim = &animCharge[LookAtPlayer()];
	anim->Reset();
	defenseCooldown = SDL_GetTicks() + DEFENSE_COOLDOWN;
	defensed = true;
}

void Enemy_Footman::doIdle()
{
	anim = &animIdle[LookAtPlayer()];
	if (DistanceToPlayer() < DISTANCE_TO_MOVE) {
		initWalk();
	}
}

void Enemy_Footman::doWalk()
{
	anim = &animWalk[LookAtPlayer()];
	if (DistanceToPlayer() > DISTANCE_TO_MOVE)
	{
		initIdle();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_ATAC)
	{
		initAtac();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_CHARGE && chargeCooldown < SDL_GetTicks() && App->entities->GetRandomNumber(10) < 3)	// superar tirada 30%
	{
		initCharge();
	}
	else if (DistanceToPlayer() < DEFENSE_DISTANCE && defenseCooldown < SDL_GetTicks() && App->entities->GetRandomNumber(10) < 3)	// superar tirada 30%
	{
		initDefense();
	}
	else // AQUI CAMINA, PERO AQUESTA FUNCIO ES TEMPORAL
	{
		if (pathVector.isEmpty())
		{
			//pathVector.CalculatePathAstar(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
			//pathVector.CalculateWay(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
			pathVector.CalculatePathAstar(iPoint((int)this->pos.x + (anim->GetCurrentRect().w / 2), (int)this->pos.y + (anim->GetCurrentRect().h / 2)), iPoint((int)App->scene->player->pos.x + (App->scene->player->anim->GetCurrentRect().w / 2), (int)App->scene->player->pos.y + (App->scene->player->anim->GetCurrentRect().h / 2)));
			pathVector.CalculateWay(iPoint((int)this->pos.x + (anim->GetCurrentRect().w / 2), (int)this->pos.y + (anim->GetCurrentRect().h / 2)), iPoint((int)App->scene->player->pos.x + (App->scene->player->anim->GetCurrentRect().w / 2), (int)App->scene->player->pos.y + (App->scene->player->anim->GetCurrentRect().h / 2)));
		}
		else
		{
			iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x, (int)pos.y));
			this->pos += fPoint((float)move.x * MOVEMENT_SPEED, (float)move.y * MOVEMENT_SPEED);
		}
	}
}

void Enemy_Footman::doAtac()
{
	if (SDL_GetTicks() > accountantPrincipal)
		initIdle();
}

void Enemy_Footman::doCharge()
{
	anim = &animCharge[LookAtPlayer()];

	if (chargeTime <= 0)
	{
		anim = &animPostCharge[saveFirstAngle];
		StopConcreteTime(1000);
		initIdle();
	}
	else
	{
		/// PER EVITAR QUE ES CAIGUI DEL MAPA
		if (App->path->ExistWalkableAtPos(iPoint(((int)pos.x + (int)chargeMovement.x) / App->map->getTileSize(), ((int)pos.y + (int)chargeMovement.y) / App->map->getTileSize())) == -1)
			accountantPrincipal = 0;
		else
		{
			pos += chargeMovement;
			chargeTime -= CHARGE_SPEED;
			App->colliders->AddTemporalCollider({ (int)pos.x, (int)pos.y, 32, 32 }, COLLIDER_TYPE::COLLIDER_ENEMY_ATAC, 10);
		}
	}
}

void Enemy_Footman::doDefense()
{
	if (SDL_GetTicks() > accountantPrincipal)
	{
		initIdle();
		defensed = false;
	}
}

// ----------------


void Enemy_Footman::ChargeAnimations()
{
	animIdle[FIXED_ANGLE::UP].PushBack			({ 913,273,76,68 });
	animIdle[FIXED_ANGLE::UP_RIGHT].PushBack	({ 305,341,76,68 });
	animIdle[FIXED_ANGLE::RIGHT].PushBack		({ 685,341,76,68 });
	animIdle[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 77,409,76,68  });
	animIdle[FIXED_ANGLE::DOWN].PushBack		({ 457,409,76,68 });
	animIdle[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 837,409,76,68 });
	animIdle[FIXED_ANGLE::LEFT].PushBack		({ 305,477,76,68 });
	animIdle[FIXED_ANGLE::UP_LEFT].PushBack		({ 609,477,76,68 });

	// -------------------------------------------------------------------

	animWalk[FIXED_ANGLE::UP].PushBack			({ 913,273,76,68 });
	animWalk[FIXED_ANGLE::UP].PushBack			({ 1,341,76,68 });
	animWalk[FIXED_ANGLE::UP].PushBack			({ 77,341,76,68 });
	animWalk[FIXED_ANGLE::UP].PushBack			({ 153,341,76,68 });
	animWalk[FIXED_ANGLE::UP].PushBack			({ 229,341,76,68 });
	animWalk[FIXED_ANGLE::UP].speed = 0.2f;

	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack	({ 305,341,76,68 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack	({ 381,341,76,68 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack	({ 457,341,76,68 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack	({ 533,341,76,68 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack	({ 609,341,76,68 });
	animWalk[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;

	animWalk[FIXED_ANGLE::RIGHT].PushBack		({ 685,341,76,68 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack		({ 761,341,76,68 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack		({ 837,341,76,68 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack		({ 913,341,76,68 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack		({ 1,409,76,68 });
	animWalk[FIXED_ANGLE::RIGHT].speed = 0.1f;

	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 77,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 153,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 229,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 305,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 381,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	
	animWalk[FIXED_ANGLE::DOWN].PushBack		({ 457,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN].PushBack		({ 533,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN].PushBack		({ 609,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN].PushBack		({ 685,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN].PushBack		({ 761,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN].speed = 0.1f;
	
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 837,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 913,409,76,68 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 1,477,76,68 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 77,477,76,68 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 153,477,76,68 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	//
	animWalk[FIXED_ANGLE::LEFT].PushBack		({ 229,477,76,68 });
	animWalk[FIXED_ANGLE::LEFT].PushBack		({ 305,477,76,68 });
	animWalk[FIXED_ANGLE::LEFT].PushBack		({ 381,477,76,68 });
	animWalk[FIXED_ANGLE::LEFT].PushBack		({ 457,477,76,68 });
	animWalk[FIXED_ANGLE::LEFT].PushBack		({ 533,477,76,68 });
	animWalk[FIXED_ANGLE::LEFT].speed = 0.1f;

	animWalk[FIXED_ANGLE::UP_LEFT].PushBack		({ 609,477,76,68 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack		({ 685,477,76,68 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack		({ 761,477,76,68 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack		({ 837,477,76,68 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack		({ 913,477,76,68 });
	animWalk[FIXED_ANGLE::UP_LEFT].speed = 0.1f;

	// -------------------------------------------------------------------

	animAtac[FIXED_ANGLE::UP].PushBack			({ 1,1,76,68 });
	animAtac[FIXED_ANGLE::UP].PushBack			({ 77,1,76,68 });
	animAtac[FIXED_ANGLE::UP].PushBack			({ 153,1,76,68 });
	animAtac[FIXED_ANGLE::UP].PushBack			({ 229,1,76,68 });
	animAtac[FIXED_ANGLE::UP].speed = 0.2f;
	animAtac[FIXED_ANGLE::UP].loop = false;

	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack	({ 305,1,76,68 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack	({ 381,1,76,68 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack	({ 457,1,76,68 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack	({ 533,1,76,68 });
	animAtac[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	animAtac[FIXED_ANGLE::UP_RIGHT].loop = false;

	animAtac[FIXED_ANGLE::RIGHT].PushBack		({ 609,1,76,68 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack		({ 685,1,76,68  });
	animAtac[FIXED_ANGLE::RIGHT].PushBack		({ 761,1,76,68 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack		({ 837,1,76,68 });
	animAtac[FIXED_ANGLE::RIGHT].speed = 0.1f;
	animAtac[FIXED_ANGLE::RIGHT].loop = false;

	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 913,1,76,68 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 1,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 77,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack	({ 153,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	animAtac[FIXED_ANGLE::DOWN_RIGHT].loop = false;

	animAtac[FIXED_ANGLE::DOWN].PushBack		({ 229,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN].PushBack		({ 305,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN].PushBack		({ 381,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN].PushBack		({ 457,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN].speed = 0.1f;
	animAtac[FIXED_ANGLE::DOWN].loop = false;

	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 533,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 609,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 685,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack	({ 761,69,76,68 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	animAtac[FIXED_ANGLE::DOWN_LEFT].loop = false;

	animAtac[FIXED_ANGLE::LEFT].PushBack		({ 837,69,76,68 });
	animAtac[FIXED_ANGLE::LEFT].PushBack		({ 913,69,76,68 });
	animAtac[FIXED_ANGLE::LEFT].PushBack		({ 1,137,76,68 });
	animAtac[FIXED_ANGLE::LEFT].PushBack		({ 77,137,76,68 });
	animAtac[FIXED_ANGLE::LEFT].speed = 0.1f;
	animAtac[FIXED_ANGLE::LEFT].loop = false;

	animAtac[FIXED_ANGLE::UP_LEFT].PushBack		({ 153,137,76,68 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack		({ 229,137,76,68 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack		({ 305,137,76,68 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack		({ 381,137,76,68 });
	animAtac[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	animAtac[FIXED_ANGLE::UP_LEFT].loop = false;

	// -------------------------------------------------------------------

	animCharge[FIXED_ANGLE::UP].PushBack({ 457,137,76,68 });
	animCharge[FIXED_ANGLE::UP].PushBack({ 533,137,76,68 });
	animCharge[FIXED_ANGLE::UP].PushBack({ 609,137,76,68 });
	animCharge[FIXED_ANGLE::UP].speed = 0.2f;
	animCharge[FIXED_ANGLE::UP].loop = false;

	animCharge[FIXED_ANGLE::UP_RIGHT].PushBack({ 685,137,76,68 });
	animCharge[FIXED_ANGLE::UP_RIGHT].PushBack({ 761,137,76,68 });
	animCharge[FIXED_ANGLE::UP_RIGHT].PushBack({ 837,137,76,68 });
	animCharge[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	animCharge[FIXED_ANGLE::UP_RIGHT].loop = false;

	animCharge[FIXED_ANGLE::RIGHT].PushBack({ 913,137,76,68 });
	animCharge[FIXED_ANGLE::RIGHT].PushBack({ 1,205,76,68 });
	animCharge[FIXED_ANGLE::RIGHT].PushBack({ 77,205,76,68 });
	animCharge[FIXED_ANGLE::RIGHT].speed = 0.1f;
	animCharge[FIXED_ANGLE::RIGHT].loop = false;

	animCharge[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 153,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 229,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 305,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	animCharge[FIXED_ANGLE::DOWN_RIGHT].loop = false;

	animCharge[FIXED_ANGLE::DOWN].PushBack({ 381,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN].PushBack({ 457,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN].PushBack({ 533,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN].speed = 0.1f;
	animCharge[FIXED_ANGLE::DOWN].loop = false;

	animCharge[FIXED_ANGLE::DOWN_LEFT].PushBack({ 609,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_LEFT].PushBack({ 685,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_LEFT].PushBack({ 761,205,76,68 });
	animCharge[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	animCharge[FIXED_ANGLE::DOWN_LEFT].loop = false;

	animCharge[FIXED_ANGLE::LEFT].PushBack({ 837,205,76,68 });
	animCharge[FIXED_ANGLE::LEFT].PushBack({ 913,205,76,68 });
	animCharge[FIXED_ANGLE::LEFT].PushBack({ 1,273,76,68 });
	animCharge[FIXED_ANGLE::LEFT].speed = 0.1f;
	animCharge[FIXED_ANGLE::LEFT].loop = false;

	animCharge[FIXED_ANGLE::UP_LEFT].PushBack({ 77,273,76,68 });
	animCharge[FIXED_ANGLE::UP_LEFT].PushBack({ 153,273,76,68 });
	animCharge[FIXED_ANGLE::UP_LEFT].PushBack({ 229,273,76,68 });
	animCharge[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	animCharge[FIXED_ANGLE::UP_LEFT].loop = false;

	animPreCharge[FIXED_ANGLE::UP].PushBack({ 305,273,76,68 });
	animPreCharge[FIXED_ANGLE::UP].speed = 0.2f;
	animPreCharge[FIXED_ANGLE::UP].loop = false;

	animPreCharge[FIXED_ANGLE::UP_RIGHT].PushBack({ 381,273,76,68 });
	animPreCharge[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::UP_RIGHT].loop = false;

	animPreCharge[FIXED_ANGLE::RIGHT].PushBack({ 457,273,76,68 });
	animPreCharge[FIXED_ANGLE::RIGHT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::RIGHT].loop = false;

	animPreCharge[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 533,273,76,68 });
	animPreCharge[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::DOWN_RIGHT].loop = false;

	animPreCharge[FIXED_ANGLE::DOWN].PushBack({ 609,273,76,68 });
	animPreCharge[FIXED_ANGLE::DOWN].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::DOWN].loop = false;

	animPreCharge[FIXED_ANGLE::DOWN_LEFT].PushBack({ 685,273,76,68 });
	animPreCharge[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::DOWN_LEFT].loop = false;

	animPreCharge[FIXED_ANGLE::LEFT].PushBack({ 761,273,76,68 });
	animPreCharge[FIXED_ANGLE::LEFT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::LEFT].loop = false;

	animPreCharge[FIXED_ANGLE::UP_LEFT].PushBack({ 837,273,76,68 });
	animPreCharge[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	animPreCharge[FIXED_ANGLE::UP_LEFT].loop = false;


	animPostCharge[FIXED_ANGLE::UP].PushBack({ 229,1,76,68 });
	animPostCharge[FIXED_ANGLE::UP].speed = 0.2f;
	animPostCharge[FIXED_ANGLE::UP].loop = false;

	animPostCharge[FIXED_ANGLE::UP_RIGHT].PushBack({ 533,1,76,68 });
	animPostCharge[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::UP_RIGHT].loop = false;

	animPostCharge[FIXED_ANGLE::RIGHT].PushBack({ 837,1,76,68 });
	animPostCharge[FIXED_ANGLE::RIGHT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::RIGHT].loop = false;

	animPostCharge[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 153,69,76,68 });
	animPostCharge[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::DOWN_RIGHT].loop = false;

	animPostCharge[FIXED_ANGLE::DOWN].PushBack({ 457,69,76,68 });
	animPostCharge[FIXED_ANGLE::DOWN].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::DOWN].loop = false;

	animPostCharge[FIXED_ANGLE::DOWN_LEFT].PushBack({ 761,69,76,68 });
	animPostCharge[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::DOWN_LEFT].loop = false;

	animPostCharge[FIXED_ANGLE::LEFT].PushBack({ 77,137,76,68 });
	animPostCharge[FIXED_ANGLE::LEFT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::LEFT].loop = false;

	animPostCharge[FIXED_ANGLE::UP_LEFT].PushBack({ 381,137,76,68 });
	animPostCharge[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	animPostCharge[FIXED_ANGLE::UP_LEFT].loop = false;
}