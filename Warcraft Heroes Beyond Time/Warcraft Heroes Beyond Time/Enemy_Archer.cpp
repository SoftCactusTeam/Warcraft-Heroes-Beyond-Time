#include "Enemy_Archer.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "ModuleInput.h"
#include "ModuleMapGenerator.h"
#include "Scene.h"
#include "ModuleInput.h"
#include "ModulePrinter.h"

#define DISTANCE_TO_MOVE	300
#define DISTANCE_TO_ATAC	150
#define ATAC_COOLDOWN		1000
#define MOVEMENT_SPEED		5
#define ARROW_SPEED			5

Enemy_Archer::Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Archer::Start()
{
	ChargeAnimations();
	state = ARCHER_STATE::ARCHER_IDLE;
	return true;
}

bool Enemy_Archer::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_9) == KeyState::KEY_DOWN)
		ShootArrow();

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

bool Enemy_Archer::PostUpdate()
{
	// L'update de les flextes
	for (int i = 0; i < arrowsVector.size(); i++)
	{
		if (arrowsVector[i]->destroy == false)
			arrowsVector[i]->Update();
		else
		{
			arrowsVector.emplace_back(arrowsVector[i]);
			arrowsVector.pop_back();
			// COMPROBAR SI AIXO FUNCIONA !!!
		}
	}
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
		pathVector.Clear();
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

void Enemy_Archer::ShootArrow()
{
	fPoint directionShoot = App->scene->player->pos;
	directionShoot.x -= pos.x;
	directionShoot.y -= pos.y;
	fPoint copyToDivideDirectionShoot = directionShoot;
	if (copyToDivideDirectionShoot.x < 0)
		copyToDivideDirectionShoot.x *= -1;
	if (copyToDivideDirectionShoot.y < 0)
		copyToDivideDirectionShoot.y *= -1;
	float total = copyToDivideDirectionShoot.x + copyToDivideDirectionShoot.y;
	if (total < 0)
		total *= -1;
	directionShoot.x /= total;
	directionShoot.y /= total;

	Enemy_Archer_Arrow* newArrow = new Enemy_Archer_Arrow(pos, App->entities->spritesheetsEntities[ARCHER_ARROW_SHEET], directionShoot * ARROW_SPEED, LookAtPlayer(), 1000);
	arrowsVector.push_back(newArrow);
}

Enemy_Archer_Arrow::Enemy_Archer_Arrow(fPoint coor, SDL_Texture* texture, fPoint direction, FIXED_ANGLE angle, int deadTimer)
{
	this->pos = coor;
	this->texture = texture;
	this->direction = direction;
	this->deadTimer = SDL_GetTicks() + deadTimer;
	this->angle = angle;

	// Assignar els rects

	rect[FIXED_ANGLE::UP] = { 0,0,32,32 };
	rect[FIXED_ANGLE::UP_RIGHT] = { 32,0,32,32 };
	rect[FIXED_ANGLE::RIGHT] = { 64,0,32,32 };
	rect[FIXED_ANGLE::DOWN_RIGHT] = { 92,0,32,32 };
	rect[FIXED_ANGLE::DOWN] = { 0,32,32,32 };
	rect[FIXED_ANGLE::DOWN_LEFT] = { 32,32,32,32 };
	rect[FIXED_ANGLE::LEFT] = { 64,32,32,32 };
	rect[FIXED_ANGLE::UP_LEFT] = { 92,32,32,32 };

}

void Enemy_Archer_Arrow::Update()
{
	if (SDL_GetTicks() < deadTimer)
	{
		this->pos += direction;
		App->printer->PrintSprite(iPoint((int)pos.x, (int)pos.y), texture, rect[angle], 2);
	}
	else
	{
		destroy = true;
	}
}

void Enemy_Archer::ChargeAnimations()
{
	//animIdle
	//north
	animIdle[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animIdle[FIXED_ANGLE::UP].speed = 0.1f;
	//north-east
	animIdle[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animIdle[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	//east
	animIdle[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animIdle[FIXED_ANGLE::RIGHT].speed = 0.1f;
	//south-east
	animIdle[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animIdle[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	//south
	animIdle[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animIdle[FIXED_ANGLE::DOWN].speed = 0.1f;
	//south-west
	animIdle[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animIdle[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	//west
	animIdle[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animIdle[FIXED_ANGLE::LEFT].speed = 0.1f;
	//north-west
	animIdle[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animIdle[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	//animWalk
	//north
	animWalk[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 142,551,44,51 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 136,496,43,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 141,445,38,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 91,493,43,48 });
	animWalk[FIXED_ANGLE::UP].speed = 0.1f;
	//north-east
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 173,211,40,43 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 141,347,42,46 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,441,43,48 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 194,306,42,45 });
	animWalk[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	//east
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 47,209,42,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 130,210,41,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 1,123,37,40 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 91,209,37,43 });
	animWalk[FIXED_ANGLE::RIGHT].speed = 0.1f;
	//south-east
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 157,41,40,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 40,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 199,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	//south
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 44,165,45,42 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 98,300,45,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 1,254,40,43 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 43,254,43,43 });
	animWalk[FIXED_ANGLE::DOWN].speed = 0.1f;
	//south-west
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 92,82,40,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 84,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 88,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 134,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	//west
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 127,255,42,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 171,256,41,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 128,123,37,40 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 88,254,37,43 });
	animWalk[FIXED_ANGLE::LEFT].speed = 0.1f;
	//north-west
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 215,211,40,43 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 185,353,42,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,393,43,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,345,42,45 });
	animWalk[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	//animAttack
	//north
	animAtac[FIXED_ANGLE::UP].PushBack({ 188,551,49,54 });
	animAtac[FIXED_ANGLE::UP].PushBack({ 94,547,46,50 });
	animAtac[FIXED_ANGLE::UP].speed = 0.1f;
	//north-east
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 181,500,45,49 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 214,256,40,48 });
	animAtac[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	//east
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,299,47,44 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,165,41,42 });
	animAtac[FIXED_ANGLE::RIGHT].speed = 0.1f;
	//south-east
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	//south
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 175,1,47,38 });
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 209,123,46,41 });
	animAtac[FIXED_ANGLE::DOWN].speed = 0.1f;
	//south-west
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 135,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 180,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	//west
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 145,301,47,44 });
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 133,166,41,42 });
	animAtac[FIXED_ANGLE::LEFT].speed = 0.1f;
	//north-west
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 1,541,45,49 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 141,395,40,48 });
	animAtac[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	//animDeath
	//north-east 
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP_RIGHT].speed = 0.1f;
	//south-east 
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,543,44,50 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 45,345,46,46 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 92,394,47,47 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].speed = 0.1f;
	//south-west
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].speed = 0.1f;
	//north-west
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 181,450,47,48 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 56,41,53,39 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 1,209,44,50 });
	animDeath[FIXED_ANGLE::UP_LEFT].speed = 0.1f;
	//north
	animDeath[FIXED_ANGLE::UP].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP].speed = 0.1f;
	//south
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN].speed = 0.1f;
	//east
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::RIGHT].speed = 0.1f;
	//west
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::LEFT].speed = 0.1f;
}