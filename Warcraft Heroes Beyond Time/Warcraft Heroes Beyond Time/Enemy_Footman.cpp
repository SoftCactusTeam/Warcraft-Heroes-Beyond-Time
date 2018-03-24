#include "Enemy_Footman.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"

#include "ModuleInput.h"

#define DISTANCE_TO_MOVE	300
#define DISTANCE_TO_CHARGE	120
#define DISTANCE_TO_ATAC	70
#define CHARGE_DISTANCE		50
#define ATAC_COOLDOWN		1000
#define MOVEMENT_SPEED		2

Enemy_Footman::Enemy_Footman(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Footman::Start()
{
	ChargeAnimations();
	state = FOOTMAN_STATE::FOOTMAN_IDLE;
	return true;
}

bool Enemy_Footman::Update(float dt)
{
	switch (state)
	{
	case FOOTMAN_STATE::FOOTMAN_IDLE:
		anim = &animIdle[LookAtPlayer()];
		if (DistanceToPlayer() < DISTANCE_TO_MOVE) {
			state = FOOTMAN_STATE::FOOTMAN_WALK;
		}
		break;
	case FOOTMAN_STATE::FOOTMAN_WALK:
		anim = &animWalk[LookAtPlayer()];
		if (DistanceToPlayer() > DISTANCE_TO_MOVE)
		{
			state = FOOTMAN_STATE::FOOTMAN_IDLE;
		}
		else if (DistanceToPlayer() < DISTANCE_TO_ATAC)
		{
			state = FOOTMAN_STATE::FOOTMAN_ATAC;
			accountantPrincipal = SDL_GetTicks() + ATAC_COOLDOWN;
			anim = &animAtac[LookAtPlayer()];
			anim->Reset();
		}
		//else if (DistanceToPlayer() < DISTANCE_TO_CHARGE)
		//{
		//	state = FOOTMAN_STATE::FOOTMAN_ATAC;
		//	accountantPrincipal = CHARGE_DISTANCE;
		//	anim = &animCharge[LookAtPlayer()];
		//	anim->Reset();
		//}
		else // AQUI CAMINA, PERO AQUESTA FUNCIO ES TEMPORAL
		{
			pos += {SillyMovementToPlayer(pos).x * MOVEMENT_SPEED, SillyMovementToPlayer(pos).y * MOVEMENT_SPEED};
		}

		break;
	case FOOTMAN_STATE::FOOTMAN_ATAC:
		if (SDL_GetTicks() > accountantPrincipal)
			state = FOOTMAN_STATE::FOOTMAN_IDLE;
		break;
	case FOOTMAN_STATE::FOOTMAN_CHARGE:
		//if (accountantPrincipal <= 0)
		//	state = FOOTMAN_STATE::FOOTMAN_IDLE;
		break;
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN)
	{
		pathVector.CalculatePathAstar(iPoint((int)this->pos.x, (int)this->pos.y), iPoint((int)App->entities->player->pos.x, (int)App->entities->player->pos.y));

	}
	return true;
}

bool Enemy_Footman::Finish()
{
	return true;
}

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
}