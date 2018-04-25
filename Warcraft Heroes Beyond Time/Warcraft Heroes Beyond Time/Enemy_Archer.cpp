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
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleItems.h"
#include "FreezeBallItem.h"

#include "ModuleRender.h"

#define DISTANCE_TO_MOVE		400
#define DISTANCE_TO_ATAC		150
#define ATAC_COOLDOWN			1000
#define TRI_ATAC_COOLDOWN		2000
#define FAST_ATAC_COOLDOWN		2000
#define FAST_ATAC_ARROWS		3
#define	FAST_ATAC_TIME_BETWEEN	100
#define MOVEMENT_SPEED			3
#define ARROW_SPEED				10
#define JUMP_BACK_COOLDOWN		300
#define DISTANCE_TO_JUMPBACK	50
#define SCAPE_TIME				5000
#define DISTANCE_TO_GO_SCAPE	60
#define ARCHER_LIVE				100
#define TIME_DYING				500
#define TEMPO_ARROW_ATWALL		500

Enemy_Archer::Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Archer::Start()
{
	ChargeAnimations();
	live = ARCHER_LIVE;
	state = ARCHER_STATE::ARCHER_IDLE;
	anim = &animIdle[LookAtPlayer()];
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

	if (false)
	{
		anim = &animIdle[DOWN];
		return true;
	}

	switch (state)
	{
	case ARCHER_STATE::ARCHER_IDLE:
		doIdle();
		break;
	case ARCHER_STATE::ARCHER_WALK:
		doWalk();
		break;
	case ARCHER_STATE::ARCHER_BASIC_ATAC:
		doAtac();
		break;
	case ARCHER_STATE::ARCHER_TRI_ATAC:
		doTriAtac();
		break;
	case ARCHER_STATE::ARCHER_FASTSHOOT_ATAC:
		doFastAtac();
		break;
	case ARCHER_STATE::ARCHER_BACKJUMP:
		doBackJump();
		break;
	case ARCHER_STATE::ARCHER_SCAPE:
		doScape();
		break;
	case ARCHER_STATE::ARCHER_DIE:
		doDie();
		break;
	case ARCHER_STATE::ARCHER_FREEZE:
		doFreeze(dt);
		break;
	default:
		initIdle();
		break;
	}

	if (damaged)
	{
		if (damagedCD - dt >= 0.0f)
		{
			damagedCD -= dt;
		}
		else
		{
			damagedCD = 0.0f;
			damaged = false;
		}	
	}

	return true;
}

bool Enemy_Archer::PostUpdate()
{
	if (App->scene->paused == true)
		anim->speed = 0.0f;
	else
		anim->speed = anim->speedFactor * App->dt;

	// ARROWS DRAW
	for (int i = 0; i < arrowsVector.size(); i++)
		if (arrowsVector[i] != nullptr)
			arrowsVector[i]->Draw();

	// AIXO ES PER COMPROBAR SI ESTA PARADA O NO
	if (stop == true)
		if (SDL_GetTicks() > accountantPrincipal)
			stop = false;
		else
		{
			// Sumes temps a la fletxa, sino es destrueix en pausa
			for (int i = 0; i < arrowsVector.size(); i++)
				arrowsVector[i]->deadTimer += App->dt * 1000;
			return true;
		}
	else if (App->scene->paused == true)
	{
		// Sumes temps a la fletxa, sino es destrueix en pausa
		for (int i = 0; i < arrowsVector.size(); i++)
			arrowsVector[i]->deadTimer += App->dt * 1000;
		return true;
	}

	// ARROWS UPDATE
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
	}
	return true;
}

bool Enemy_Archer::Finish()
{
	return true;
}

bool Enemy_Archer::Draw()
{
	bool ret = true;
	if(damaged)
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot(), ModulePrinter::Pivots::UPPER_LEFT, {0,0}, 0, { 255,100,100,255 });
	else
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());
	return ret;
}

void Enemy_Archer::OnCollision(Collider* yours, Collider* collideWith)
{
	if (collideWith->colType == Collider::ColliderType::PLAYER_ATTACK)
	{
		PlayerAttack* attack = (PlayerAttack*)collideWith;

		switch (attack->pattacktype)
		{
			case PlayerAttack::P_Attack_Type::NORMAL_ATTACK:
			case PlayerAttack::P_Attack_Type::SKILL:
			case PlayerAttack::P_Attack_Type::DMGBALL_ITEM:
			{
				App->audio->PlayFx(App->audio->ArcherDeath);
				live -= attack->damage;
				if (live <= 0)
				{
					if (state != ARCHER_STATE::ARCHER_DIE)
						initDie();
				}
				else
					initBackJump();

				break;
			}
			case PlayerAttack::P_Attack_Type::FREEZE_ITEM:
			{
				if (state != ARCHER_STATE::ARCHER_FREEZE && App->entities->GetRandomNumber(10) < 2)
				{
					initFreeze();
				}
				break;
			}
			case PlayerAttack::P_Attack_Type::SHIT:
			{
				live -= attack->damage;
				if (live <= 0)
					if (state != ARCHER_STATE::ARCHER_DIE)
						initDie();
			}
		}
	
		if (attack->damage > 0)
		{
			damaged = true;
			damagedCD = 0.5f;
		}
	}
}

void Enemy_Archer::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
	if (collideWith->colType == Collider::ColliderType::PLAYER_ATTACK)
	{
		PlayerAttack* attack = (PlayerAttack*)collideWith;
		switch (attack->pattacktype)
		{
			case PlayerAttack::P_Attack_Type::SHIT:
			{
				live -= attack->damage;
				if (live <= 0)
					if (state != ARCHER_STATE::ARCHER_DIE)
						initDie();
			}
		}
	}
}

// ---------------------------------------------------------------------------------
// ----------------  INIT STATE_MACHIN FUNCTIONS  ----------------------------------
// ---------------------------------------------------------------------------------

void Enemy_Archer::initIdle()
{
	state = ARCHER_STATE::ARCHER_IDLE;
	pathVector.Clear();
}

void Enemy_Archer::initWalk()
{
	state = ARCHER_STATE::ARCHER_WALK;
}

void Enemy_Archer::initAtac()
{
	state = ARCHER_STATE::ARCHER_BASIC_ATAC;
	accountantPrincipal = SDL_GetTicks() + ATAC_COOLDOWN;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
	ShootArrow();
}

void Enemy_Archer::initTriAtac()
{
	state = ARCHER_STATE::ARCHER_TRI_ATAC;
	accountantPrincipal = SDL_GetTicks() + TRI_ATAC_COOLDOWN;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
	ShootArrow();
	ShootArrow({ 20,-20 });
	ShootArrow({ -20,20 });
}

void Enemy_Archer::initFastAtac()
{
	state = ARCHER_STATE::ARCHER_FASTSHOOT_ATAC;
	accountantPrincipal = SDL_GetTicks() + FAST_ATAC_COOLDOWN;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
	arrowToShoot = FAST_ATAC_ARROWS;
	timeToShootAnother = SDL_GetTicks() + FAST_ATAC_TIME_BETWEEN;
}

void Enemy_Archer::initBackJump()
{
	state = ARCHER_STATE::ARCHER_BACKJUMP;
	accountantPrincipal = SDL_GetTicks() + JUMP_BACK_COOLDOWN;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();

	for (int i = 0; i < 10; i++)
	{
		int randomX = App->entities->GetRandomNumber(6);
		if (randomX > 3)
		{
			randomX -= 3;
			randomX *= -1;
		}
		int randomY = App->entities->GetRandomNumber(6);
		if (randomY > 3)
		{
			randomY -= 3;
			randomY *= -1;
		}
		int numPos;
		if (App->path->ExistWalkableAtPos(iPoint(((int)pos.x)/ (App->map->getTileSize() - 2) + randomX, ((int)pos.y) / (App->map->getTileSize() - 2) + randomY)) != -1)
		{
			numPos = App->path->ExistWalkableAtPos(iPoint(((int)pos.x) / (App->map->getTileSize() - 2) + randomX, ((int)pos.y) / (App->map->getTileSize() - 2) + randomY));
			posSmoke = pos;
			tempoSmoke = 300 + SDL_GetTicks();
			pos.x = App->path->map[numPos]->nodePos.x * (App->map->getTileSize() - 2);
			pos.y = App->path->map[numPos]->nodePos.y * (App->map->getTileSize() - 2);
			break;
		}
	}
	animSmoke.Reset();
}

void Enemy_Archer::initScape()
{
	int randomX = App->entities->GetRandomNumber(DISTANCE_TO_GO_SCAPE);
	if (randomX > DISTANCE_TO_GO_SCAPE / 2)
	{
		randomX -= DISTANCE_TO_GO_SCAPE / 2;
		randomX *= -1;
	}
	int randomY = App->entities->GetRandomNumber(DISTANCE_TO_GO_SCAPE);
	if (randomY > DISTANCE_TO_GO_SCAPE / 2)
	{
		randomY -= DISTANCE_TO_GO_SCAPE / 2;
		randomY *= -1;
	}
	bool finded = true;
	while (App->path->ExistWalkableAtPos(iPoint((int)pos.x / App->map->getTileSize() - randomX, (int)pos.y / App->map->getTileSize() - randomY)) == false)
	{
		randomX--;
		randomY--;
		if (randomX < DISTANCE_TO_GO_SCAPE / 4 || randomY < DISTANCE_TO_GO_SCAPE / 4) {
			finded = false;
			break;
		}
	}
	if (finded)
	{
		posToScape = { iPoint((int)pos.x / App->map->getTileSize() - randomX , (int)pos.y / App->map->getTileSize() - randomY) };
		state = ARCHER_STATE::ARCHER_SCAPE;
		accountantPrincipal = SDL_GetTicks() + SCAPE_TIME;
		anim = &animAtac[LookAtPlayer()];
		anim->Reset();
		pathVector.Clear();
	}
	else
	{
		state = ARCHER_STATE::ARCHER_IDLE;
		pathVector.Clear();
	}
}

void Enemy_Archer::initDie()
{
	state = ARCHER_STATE::ARCHER_DIE;
	accountantPrincipal = SDL_GetTicks() + TIME_DYING;
	anim = &animDeath[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
}

void Enemy_Archer::initFreeze()
{
	state = ARCHER_STATE::ARCHER_FREEZE;
	pathVector.Clear();
}

// ---------------------------------------------------------------------------------
// ----------------  UPDATE(DO) STATE_MACHIN FUNCTIONS  ----------------------------
// ---------------------------------------------------------------------------------

void Enemy_Archer::doIdle()
{
	anim = &animIdle[LookAtPlayer()];
	if (DistanceToPlayer() < DISTANCE_TO_MOVE) {
		initWalk();
	}
}

void Enemy_Archer::doWalk()
{
	anim = &animWalk[LookAtPlayer()];
	if (DistanceToPlayer() > DISTANCE_TO_MOVE)
	{
		initIdle();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_JUMPBACK && App->entities->GetRandomNumber(10) < 7)	// Superar tirada 70%
	{
		initBackJump();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_ATAC && App->entities->GetRandomNumber(10) < 7)	// Superar una tirada de 70%
	{
		initAtac();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_ATAC && App->entities->GetRandomNumber(10) < 5)	// Superar una tirada de 50%
	{
		initTriAtac();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_ATAC && App->entities->GetRandomNumber(10) < 5)	// Superar una tirada de 50%
	{
		initFastAtac();
	}
	//else if (DistanceToPlayer() < DISTANCE_TO_JUMPBACK /*is the same to scape*/ /*&& App->entities->GetRandomNumber(10) < 7*/)	// Superar tirada 70%
	//{
	//initScape();
	//}
	else // AQUI CAMINA, PERO AQUESTA FUNCIO ES TEMPORAL
	{
		if (pathVector.isEmpty())
		{
			if (pathVector.CalculatePathAstar(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y)))
				pathVector.CalculateWay(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
		}
		else
		{
			iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x + (anim->GetCurrentRect().w / 2), (int)pos.y + (anim->GetCurrentRect().h / 2)));
			this->pos += fPoint((float)move.x * MOVEMENT_SPEED, (float)move.y * MOVEMENT_SPEED);
		}
	}
}

void Enemy_Archer::doAtac()
{
	if (SDL_GetTicks() > accountantPrincipal)
		initIdle();
}

void Enemy_Archer::doTriAtac()
{
	if (SDL_GetTicks() > accountantPrincipal)
		initIdle();
}

void Enemy_Archer::doFastAtac()
{
	if (SDL_GetTicks() > accountantPrincipal)
		initIdle();

	else if (arrowToShoot > 0)
	{
		if (timeToShootAnother < SDL_GetTicks())
		{
			ShootArrow();
			timeToShootAnother = SDL_GetTicks() + FAST_ATAC_TIME_BETWEEN;
			arrowToShoot--;
		}
	}
}

void Enemy_Archer::doBackJump()
{
	if (SDL_GetTicks() > accountantPrincipal && tempoSmoke < SDL_GetTicks())
		initIdle();
	else
		App->printer->PrintSprite(iPoint((int)posSmoke.x, (int)posSmoke.y), App->entities->spritesheetsEntities[ARCHER_SMOKE_SHEET], animSmoke.GetCurrentFrame(), 2);
}

void Enemy_Archer::doScape()
{
	if (SDL_GetTicks() > accountantPrincipal)
		initIdle();

	if (pathVector.isEmpty())
	{
		pathVector.CalculatePathAstar(iPoint((int)this->pos.x, (int)this->pos.y), iPoint(posToScape.x * App->map->getTileSize(), posToScape.y* App->map->getTileSize()));
		pathVector.CalculateWay(iPoint((int)this->pos.x, (int)this->pos.y), iPoint(posToScape.x* App->map->getTileSize(), posToScape.y* App->map->getTileSize()));
	}
	else
	{
		iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x, (int)pos.y));
		this->pos += fPoint((float)move.x * MOVEMENT_SPEED, (float)move.y * MOVEMENT_SPEED);
	}
}

void Enemy_Archer::doDie()
{
	if (SDL_GetTicks() > accountantPrincipal)
	{
		destroy = true;
		App->entities->enemiescount--;
	}
		
}

void Enemy_Archer::doFreeze(float dt)
{
	//anim = &animFrozen[LookAtPlayer()];//Put animation here
	frozen_counter += dt;
	if (frozen_counter > 5.0f)
	{
		state = ARCHER_STATE::ARCHER_IDLE;
		frozen_counter = 0.0f;
	}
}

void Enemy_Archer::ShootArrow(fPoint desviation)
{
	App->audio->PlayFx(App->audio->ArrowSound);
	fPoint directionShoot = App->scene->player->pos;

	directionShoot.x -= pos.x + desviation.x;
	directionShoot.y -= pos.y + desviation.y;
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
	fPoint position = fPoint(pos.x + anim->GetCurrentRect().w / 2, pos.y + anim->GetCurrentRect().h / 2);
	Enemy_Archer_Arrow* newArrow = new Enemy_Archer_Arrow(position, App->entities->spritesheetsEntities[PROJECTILE_SHEET], directionShoot * ARROW_SPEED, 1000);
	arrowsVector.push_back(newArrow);
}

Enemy_Archer_Arrow::Enemy_Archer_Arrow(fPoint coor, SDL_Texture* texture, fPoint direction, int deadTimer)
{
	this->pos = coor;
	this->texture = texture;
	this->direction = direction;
	this->deadTimer = SDL_GetTicks() + deadTimer;
	this->angle = atan2(coor.y - App->scene->player->pos.y, coor.x - App->scene->player->pos.x);
	if (angle > 0)
		angle = angle * 360 / (2 * PI);
	else
		angle = (2 * PI + angle) * 360 / (2 * PI);
	angle -= 90;

	tempoAtWall = -1;
	//We need the new module for that
	//arrowCollider = App->colliders->AddEnemyAttackCollider({ (int)coor.x,(int)coor.y,5,5 }, this, Collider::ColliderType::ENEMY_ATTACK, nullptr, { 0,0 });
	rect = { 808,110,32,32 };
}

void Enemy_Archer_Arrow::Update()
{
	if (tempoAtWall != -1)
	{
		if (tempoAtWall < SDL_GetTicks())
			destroy = true;
	}
	else if (SDL_GetTicks() < deadTimer)
	{
		this->pos += direction;
		/*arrowCollider->colliderRect.x = (int)pos.x;
		arrowCollider->colliderRect.y = (int)pos.y;
		if (arrowCollider->collidingWith != nullptr)
		{
			if ((arrowCollider->collidingWith->type == COLLIDER_TYPE::COLLIDER_PLAYER ||
				(arrowCollider->collidingWith->type == COLLIDER_TYPE::COLLIDER_PLAYER_ATTACK && arrowCollider->collidingWith->attackType != Collider::ATTACK_TYPE::SHIT)))
			{
				destroy = true;
			}
			else if (arrowCollider->collidingWith->type == COLLIDER_TYPE::COLLIDER_UNWALKABLE)
			{
				tempoAtWall = TEMPO_ARROW_ATWALL + SDL_GetTicks();
				Finish();
			}
		}*/
			
	}
	else
		destroy = true;
}

void Enemy_Archer_Arrow::Draw()
{
	App->printer->PrintSprite(iPoint((int)pos.x, (int)pos.y), texture, rect, 2, ModulePrinter::Pivots::CENTER, { 0,0 }, ModulePrinter::Pivots::UPPER_LEFT, {0,0}, angle);
}



void Enemy_Archer_Arrow::Finish()
{
	App->colliders->deleteCollider(arrowCollider);
}

void Enemy_Archer::ChargeAnimations()
{
	//animIdle
	//north
	animIdle[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animIdle[FIXED_ANGLE::UP].speedFactor = 9.0f;
	//north-east
	animIdle[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animIdle[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	//east
	animIdle[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animIdle[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	//south-east
	animIdle[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animIdle[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	//south
	animIdle[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animIdle[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	//south-west
	animIdle[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animIdle[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	//west
	animIdle[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animIdle[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	//north-west
	animIdle[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animIdle[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;
	//animWalk
	//north
	animWalk[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 142,551,44,51 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 136,496,43,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 141,445,38,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 91,493,43,48 });
	animWalk[FIXED_ANGLE::UP].speedFactor = 9.0f;
	//north-east
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 173,211,40,43 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 141,347,42,46 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,441,43,48 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 194,306,42,45 });
	animWalk[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	//east
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 47,209,42,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 130,210,41,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 1,123,37,40 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 91,209,37,43 });
	animWalk[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	//south-east
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 157,41,40,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 40,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 199,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	//south
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 44,165,45,42 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 98,300,45,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 1,254,40,43 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 43,254,43,43 });
	animWalk[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	//south-west
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 92,82,40,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 84,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 88,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 134,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	//west
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 127,255,42,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 171,256,41,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 128,123,37,40 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 88,254,37,43 });
	animWalk[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	//north-west
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 215,211,40,43 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 185,353,42,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,393,43,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,345,42,45 });
	animWalk[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;
	//animAttack
	//north
	animAtac[FIXED_ANGLE::UP].PushBack({ 188,551,49,54 });
	animAtac[FIXED_ANGLE::UP].PushBack({ 94,547,46,50 });
	animAtac[FIXED_ANGLE::UP].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::UP].loop = false;

	//north-east
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 181,500,45,49 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 214,256,40,48 });
	animAtac[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::UP_RIGHT].loop = false;

	//east
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,299,47,44 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,165,41,42 });
	animAtac[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::RIGHT].loop = false;

	//south-east
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::DOWN_RIGHT].loop = false;

	//south
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 175,1,47,38 });
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 209,123,46,41 });
	animAtac[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::DOWN].loop = false;

	//south-west
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 135,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 180,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::DOWN_LEFT].loop = false;

	//west
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 145,301,47,44 });
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 133,166,41,42 });
	animAtac[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::LEFT].loop = false;

	//north-west
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 1,541,45,49 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 141,395,40,48 });
	animAtac[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;
	animAtac[FIXED_ANGLE::UP_LEFT].loop = false;


	//animDeath
	//north-east 
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP_RIGHT].loop = false;
	//south-east 
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,543,44,50 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 45,345,46,46 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 92,394,47,47 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN_RIGHT].loop = false;
	//south-west
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN_LEFT].loop = false;
	//north-west
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 181,450,47,48 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 56,41,53,39 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 1,209,44,50 });
	animDeath[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP_LEFT].loop = false;
	//north
	animDeath[FIXED_ANGLE::UP].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP].loop = false;
	//south
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN].loop = false;
	//east
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::RIGHT].loop = false;
	//west
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::LEFT].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::LEFT].loop = false;

	//animFrozen
	//All the same animation
	/*animFrozen[FIXED_ANGLE::LEFT].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::RIGHT].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::DOWN].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::UP].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::DOWN_LEFT].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::UP_LEFT].PushBack({ 1,491,44,48 });
	animFrozen[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,491,44,48 });*/


	animSmoke.PushBack({ 1,77,52,42 });
	animSmoke.PushBack({ 151,65,56,60 });
	animSmoke.PushBack({ 151,1,57,62 });
	animSmoke.PushBack({ 79,1,70,70 });
	animSmoke.PushBack({ 1,1,76,74 });
	animSmoke.PushBack({ 79,1,70,70 });
	animSmoke.PushBack({ 151,1,57,62 });
	animSmoke.PushBack({ 151,65,56,60 });
	animSmoke.PushBack({ 1,77,52,42 });
	animSmoke.speedFactor = 9.0f;
	animSmoke.loop = true;
}
