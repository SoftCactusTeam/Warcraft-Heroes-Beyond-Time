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

#include "Projectile.h"
#include "ArcherArrow.h"
#include "FreezeBallItem.h"


#include "ModuleRender.h"

// DON'T DO
#define TRI_ATAC_COOLDOWN		2000	// treure
#define FAST_ATAC_COOLDOWN		2000	// treure fast atac
#define FAST_ATAC_ARROWS		3		// treure fast atac
#define	FAST_ATAC_TIME_BETWEEN	100		// treure fast atac
#define JUMP_BACK_COOLDOWN		300		// no es fara
#define DISTANCE_TO_JUMPBACK	50		// no es fara

// DON'T TOUCH
#define TEMPO_ARROW_ATWALL		500		// es deixa aixi
#define TIME_DYING				500		// es deixa aixi
#define LITTLEMOVEMENT_TIME		100		// es deixa aixi
#define LITTLEMOVEMENT_COOLDOWN	1000	// res

#define TIME_STUNNED_AFTERHIT	1000	// in ms
#define VELOCITY_DASH			500		// in pixels per sex
#define TIMING_DASH				300		// in ms


Enemy_Archer::Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture)
{
	switch (character)
	{
	case ENEMY_TYPE::ARCHER_TIER_1:
		numStats = App->entities->archerT1stats;
		this->tier = 1;
		break;
	case ENEMY_TYPE::ARCHER_TIER_2:
		numStats = App->entities->archerT2stats;
		this->tier = 2;
		break;
	case ENEMY_TYPE::ARCHER_TIER_3:
		numStats = App->entities->archerT3stats;
		this->tier = 3;
		break;
	}

	//USAR SOLO VARIABLES EN NUMSTATS, SI SE NECESITA ALGUNA M�S SE COMENTA CON EL EQUIPO Y SE DECIDE SI SE A�ADE. TODO CONFIGURABLE DESDE EL XML.
}

bool Enemy_Archer::Start()
{
	LoadAnimations();
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
	case ARCHER_STATE::ARCHER_LITTLEMOVE:
		doLittleMove();
		break;
	case ARCHER_STATE::ARCHER_DASH:
		doDash();
		break;
	case ARCHER_STATE::ARCHER_DIE:
		doDie();
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
	return true;
}

bool Enemy_Archer::Finish()
{
	return true;
}

bool Enemy_Archer::Draw()
{
	bool ret = true;
	
	switch (tier)
	{
	case 1:
		if(damaged)
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[WHITE_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot(), ModulePrinter::Pivots::UPPER_LEFT, { 0,0 }, 0, { 255,100,100,255 });
		else
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[WHITE_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());
		break;
	case 2:
		if (damaged)
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[ORANGE_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot(), ModulePrinter::Pivots::UPPER_LEFT, { 0,0 }, 0, { 255,100,100,255 });
		else
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[ORANGE_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());
		break;
	case 3:
		if (damaged)
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[GREEN_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot(), ModulePrinter::Pivots::UPPER_LEFT, { 0,0 }, 0, { 255,100,100,255 });
		else
			ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), App->entities->spritesheetsEntities[GREEN_ARCHER], anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());
		break;
	}
	
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
				initDash();
			case PlayerAttack::P_Attack_Type::DMGBALL_ITEM:
			{
				App->audio->PlayFx(App->audio->ArcherDamaged);
				numStats.hp -= attack->damage;;
				if (numStats.hp <= 0)
				{
					if (state != ARCHER_STATE::ARCHER_DIE)
						initDie();
				}
				break;
			}
			case PlayerAttack::P_Attack_Type::FREEZE_ITEM:
			{
				/*if (state != ARCHER_STATE::ARCHER_FREEZE && App->entities->GetRandomNumber(10) < 2)
				{
					initFreeze();
				}*/
				break;
			}
			case PlayerAttack::P_Attack_Type::FEARBALL_ITEM:
			{
				/*if (state != ARCHER_STATE::ARCHER_FEAR)
					initFear();*/
				break;
			}
			case PlayerAttack::P_Attack_Type::SHIT:
			{
				numStats.hp -= attack->damage;
				if (numStats.hp <= 0)
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
				numStats.hp -= attack->damage;
				if (numStats.hp <= 0)
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
	accountantPrincipal = SDL_GetTicks() + numStats.preAttac;
	anim = &animAtac[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
	hasAttacked = false;
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
	arrowsShooted++;
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
	arrowsShooted++;
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

void Enemy_Archer::initLittleMove()
{
	int randomX = 0;
	int randomY = 0;
	int tileToMove = -1;

	for (int i = 0; i < 10 && tileToMove == -1; i++)
	{
		randomX = rand() % numStats.tilesToLittleMove + 1;
		if (randomX > numStats.tilesToLittleMove / 2)
		{
			randomX -= numStats.tilesToLittleMove / 2;
			randomX *= -1;
		}

		randomY = rand() % numStats.tilesToLittleMove + 1;
		if (randomY > numStats.tilesToLittleMove / 2)
		{
			randomY -= numStats.tilesToLittleMove / 2;
			randomY *= -1;
		}

		int possibleTile = App->path->ExistWalkableAtPos(iPoint((int)pos.x / App->map->getTileSize() - randomX, (int)pos.y / App->map->getTileSize() - randomY));
		if (possibleTile != -1)
			//if (App->scene->player->DistanceToObejective(App->path->posAtConcreteTile(possibleTile)) < App->scene->player->DistanceToObejective(App->path->posAtConcreteTile(tileToMove)))
 			//	tileToMove = possibleTile;
			//else if (tileToMove == -1)
				tileToMove = possibleTile;
	}

	if (tileToMove != -1)
	{
		posToScape = { iPoint((int)pos.x / App->map->getTileSize() - randomX , (int)pos.y / App->map->getTileSize() - randomY) };
		state = ARCHER_STATE::ARCHER_LITTLEMOVE;
		accountantPrincipal = SDL_GetTicks() + LITTLEMOVEMENT_TIME;
		anim = &animWalk[LookAtPlayer()];
		anim->Reset();
		pathVector.CalculatePathAstar(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint(posToScape.x * App->map->getTileSize(), posToScape.y* App->map->getTileSize()));
		pathVector.CalculateWay(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint(posToScape.x* App->map->getTileSize(), posToScape.y* App->map->getTileSize()));
		arrowsShooted = 0;
		cooldownToReLittleMove = LITTLEMOVEMENT_COOLDOWN + SDL_GetTicks();
	}
	else
	{
		initBackJump();
	}
}

void Enemy_Archer::initDash()
{
	state = ARCHER_STATE::ARCHER_DASH;
	anim = &animIdle[LookAtPlayer()];
	anim->Reset();
	saveFirstAngle = LookAtPlayer();
	dashTempo = 0;
	dashMovement = transformFixedAngleTofPoint(App->scene->player->returnFixedAngle());
	//dashMovement = CaculateFPointAngle(fPoint(App->scene->player->pos.x + (App->scene->player->anim->GetCurrentRect().w / 3) , App->scene->player->pos.y + (App->scene->player->anim->GetCurrentRect().h / 3)), anim->GetCurrentRect().w / 2, anim->GetCurrentRect().h / 2);
	dashMovement.x *= 1 * (numStats.velocityDashHit) / 3 * App->dt;
	dashMovement.y *= 1 * (numStats.velocityDashHit) / 3 * App->dt;
}

void Enemy_Archer::initDie()
{
	state = ARCHER_STATE::ARCHER_DIE;
	accountantPrincipal = SDL_GetTicks() + TIME_DYING;
	anim = &animDeath[LookAtPlayer()];
	anim->Reset();
	pathVector.Clear();
}

// ---------------------------------------------------------------------------------
// ----------------  UPDATE(DO) STATE_MACHIN FUNCTIONS  ----------------------------
// ---------------------------------------------------------------------------------

void Enemy_Archer::doIdle()
{
	anim = &animIdle[LookAtPlayer()];
	if (DistanceToPlayer() < numStats.vision_range) {
		initWalk();
	}
}

// FUNCIO CENTRAL DE L'ARCHER !!!
void Enemy_Archer::doWalk()
{
	anim = &animWalk[LookAtPlayer()];

	if (App->entities->checkEntityNearOther(this, numStats.DistanceToScape) && state != ARCHER_STATE::ARCHER_LITTLEMOVE && pathVector.isEmpty())
		initLittleMove();
	else if (DistanceToPlayer() > numStats.vision_range)
	{
		initIdle();
	}
	else if (DistanceToPlayer() < numStats.minimal_distance_player /*&& cooldownToReLittleMove < SDL_GetTicks()*/ && arrowsShooted > 0)
	{
		initLittleMove();
	}
	else if (DistanceToPlayer() < DISTANCE_TO_JUMPBACK && App->entities->GetRandomNumber(10) < 0)	// Ara mateix no fa backJump
	{
		initBackJump();
	}
	else if (DistanceToPlayer() < numStats.attack_range && App->entities->GetRandomNumber(100) < numStats.attacCone_probability)
	{
		initTriAtac();
	}
	else if (DistanceToPlayer() < numStats.attack_range && App->entities->GetRandomNumber(100) < numStats.attacFast_probability)
	{
		initFastAtac();
	}
	else if (DistanceToPlayer() < numStats.attack_range/* && App->entities->GetRandomNumber(100) < 100*/)
	{
		initAtac();
	}
	else
	{
		Walk();
	}
}

void Enemy_Archer::doAtac()
{
	if (hasAttacked)
	{
		if (SDL_GetTicks() > accountantPrincipal)
			initIdle();
	}
	else
	{
		if (SDL_GetTicks() > accountantPrincipal)
		{
			hasAttacked = true;
			accountantPrincipal = SDL_GetTicks() + numStats.time_between_attacks;
			ShootArrow();
			arrowsShooted++;
		}
		anim = &animAtac[LookAtPlayer()];
	}
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

void Enemy_Archer::doLittleMove()
{
	if (pathVector.isEmpty())
	{
		initIdle();
	}
		iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x + (anim->GetCurrentRect().w / 2), (int)pos.y + (anim->GetCurrentRect().h / 2)));
		this->pos += fPoint((float)move.x * numStats.speed, (float)move.y * numStats.speed);
}

void Enemy_Archer::doDash()
{
	anim = &animIdle[saveFirstAngle];

	if (dashTempo * 1000 >= numStats.timingDashHit)
	{
		StopConcreteTime(numStats.timeStunedAfterHit);
		initIdle();
	}
	else
	{
		// PER EVITAR QUE ES CAIGUI DEL MAPA
		switch (saveFirstAngle)
		{
		case FIXED_ANGLE::UP:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w / 2 +  dashMovement.x) / App->map->getTileSize(), (pos.y + anim->GetCurrentRect().h + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::UP_RIGHT:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w + dashMovement.x) / App->map->getTileSize(), (pos.y + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::RIGHT:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w + dashMovement.x) / App->map->getTileSize(), (pos.y + anim->GetCurrentRect().h / 2 + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
			case FIXED_ANGLE::DOWN_RIGHT:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w + dashMovement.x) / App->map->getTileSize(), (pos.y + anim->GetCurrentRect().h + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::DOWN:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w / 2 + dashMovement.x) / App->map->getTileSize(), (pos.y + anim->GetCurrentRect().h + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::DOWN_LEFT:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + dashMovement.x) / App->map->getTileSize(), (pos.y + anim->GetCurrentRect().h + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::LEFT:
			if (App->path->ExistWalkableAtPos(iPoint((pos.x + anim->GetCurrentRect().w / 2 + dashMovement.x) / App->map->getTileSize(), (pos.y + dashMovement.y) / App->map->getTileSize())) == -1)
				dashTempo = numStats.timingDashHit;	// break
			break;
		case FIXED_ANGLE::UP_LEFT:
				if (App->path->ExistWalkableAtPos(iPoint((pos.x + dashMovement.x) / App->map->getTileSize(), (pos.y + dashMovement.y) / App->map->getTileSize())) == -1)
					dashTempo = numStats.timingDashHit;	// break
				break;
		}
		if (dashTempo != numStats.timingDashHit)
		{
			pos += dashMovement;
			dashTempo += App->dt;
		}
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

void Enemy_Archer::Walk()
{
	if (pathVector.isEmpty())
	{
		if (pathVector.CalculatePathAstar(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y)))
			pathVector.CalculateWay(iPoint(((int)pos.x + (anim->GetCurrentRect().w / 2)), ((int)pos.y + (anim->GetCurrentRect().h / 2))), iPoint((int)App->scene->player->pos.x, (int)App->scene->player->pos.y));
	}
	else
	{
		iPoint move = pathVector.nextTileToMove(iPoint((int)pos.x + (anim->GetCurrentRect().w / 2), (int)pos.y + (anim->GetCurrentRect().h / 2)));
		this->pos += fPoint((float)move.x * numStats.speed, (float)move.y * numStats.speed);
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

void Enemy_Archer::AddEffect(ARCHER_EFFECTS effect, int time)
{
	archerEffectStruct* aux = new archerEffectStruct();
	aux->effect = effect;
	aux->time = time;
	effectsList.push_back(aux);
	switch (effect)		// INITIAL EFFECT
	{
	case ARCHER_EFFECT_FREEZE:

		break;
	case ARCHER_EFFECT_BURNING:

		break;
	case ARCHER_EFFECT_NONE:
		break;
	}
}

void Enemy_Archer::UpdateEffects()
{
	std::list<archerEffectStruct*>::iterator it = effectsList.begin();
	for (; it != effectsList.end(); it++)
	{
		if ((*it)->time < SDL_GetTicks())
		{			// UPDATE
			switch ((*it)->effect)
			{
			case ARCHER_EFFECT_FREEZE:

				break;
			case ARCHER_EFFECT_BURNING:

				break;
			case ARCHER_EFFECT_NONE:

				break;
			}
		}
		else
		{			// END & FINAL EFFECT
			switch ((*it)->effect)
			{
			case ARCHER_EFFECT_FREEZE:

				break;
			case ARCHER_EFFECT_BURNING:

				break;
			case ARCHER_EFFECT_NONE:

				break;
			}
			effectsList.erase(it++);	// DELETE CONCRETE EFFECT
		}
	}
}

void Enemy_Archer::ShootArrow(fPoint desviation)
{
	App->audio->PlayFx(App->audio->ArrowSound);
	fPoint directionShoot = fPoint(App->scene->player->pos.x/* + App->scene->player->anim->GetCurrentRect().w / 2*/ , App->scene->player->pos.y/* + App->scene->player->anim->GetCurrentRect().h / 2*/);
	directionShoot.x -= (pos.x + 16/*Arrow Width & Height*/) + desviation.x;
	directionShoot.y -= (pos.y + 16) + desviation.y;

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
	/*switch (LookAtPlayer())
	{
	case FIXED_ANGLE::UP:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 0, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::UP_RIGHT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::RIGHT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::DOWN:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::LEFT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 5, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	case FIXED_ANGLE::UP_LEFT:
		position = fPoint(pos.x + anim->GetCurrentRect().w / 2 + 0, pos.y + anim->GetCurrentRect().h / 2 + 5);
		break;
	}*/

	// POSAR FLETXA
	ArcherArrowInfo info;
	info.pos = position;
	info.direction = directionShoot;
	info.deadTimer = numStats.arrows_life;
	info.speed = numStats.arrows_speed;
	info.damageArrow = numStats.damage;

	App->projectiles->AddProjectile(&info, Projectile_type::archer_arrow);
}

void Enemy_Archer::LoadAnimations()
{
	animIdle[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animIdle[FIXED_ANGLE::UP].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animIdle[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animIdle[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animIdle[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animIdle[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animIdle[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animIdle[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	animIdle[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animIdle[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;


	animWalk[FIXED_ANGLE::UP].PushBack({ 47,491,42,48 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 142,551,44,51 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 136,496,43,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 141,445,38,49 });
	animWalk[FIXED_ANGLE::UP].PushBack({ 91,493,43,48 });
	animWalk[FIXED_ANGLE::UP].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 46,393,44,47 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 173,211,40,43 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 141,347,42,46 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,441,43,48 });
	animWalk[FIXED_ANGLE::UP_RIGHT].PushBack({ 194,306,42,45 });
	animWalk[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 167,123,40,41 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 47,209,42,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 130,210,41,43 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 1,123,37,40 });
	animWalk[FIXED_ANGLE::RIGHT].PushBack({ 91,209,37,43 });
	animWalk[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 111,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 157,41,40,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 40,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 199,41,44,39 });
	animWalk[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 50,299,46,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 44,165,45,42 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 98,300,45,44 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 1,254,40,43 });
	animWalk[FIXED_ANGLE::DOWN].PushBack({ 43,254,43,43 });
	animWalk[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 46,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 92,82,40,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 84,123,42,40 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 88,1,45,38 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].PushBack({ 134,82,44,39 });
	animWalk[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 91,165,40,42 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 127,255,42,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 171,256,41,43 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 128,123,37,40 });
	animWalk[FIXED_ANGLE::LEFT].PushBack({ 88,254,37,43 });
	animWalk[FIXED_ANGLE::LEFT].speedFactor = 9.0f;
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 46,442,44,47 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 215,211,40,43 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 185,353,42,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,393,43,46 });
	animWalk[FIXED_ANGLE::UP_LEFT].PushBack({ 1,345,42,45 });
	animWalk[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;


	animAtac[FIXED_ANGLE::UP].PushBack({ 188,551,49,54 });
	animAtac[FIXED_ANGLE::UP].PushBack({ 94,547,46,50 });
	animAtac[FIXED_ANGLE::UP].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::UP].loop = false;
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 181,500,45,49 });
	animAtac[FIXED_ANGLE::UP_RIGHT].PushBack({ 214,256,40,48 });
	animAtac[FIXED_ANGLE::UP_RIGHT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::UP_RIGHT].loop = false;
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,299,47,44 });
	animAtac[FIXED_ANGLE::RIGHT].PushBack({ 1,165,41,42 });
	animAtac[FIXED_ANGLE::RIGHT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::RIGHT].loop = false;
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 1,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::DOWN_RIGHT].loop = false;
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 175,1,47,38 });
	animAtac[FIXED_ANGLE::DOWN].PushBack({ 209,123,46,41 });
	animAtac[FIXED_ANGLE::DOWN].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::DOWN].loop = false;
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 135,1,38,38 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].PushBack({ 180,82,43,39 });
	animAtac[FIXED_ANGLE::DOWN_LEFT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::DOWN_LEFT].loop = false;
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 145,301,47,44 });
	animAtac[FIXED_ANGLE::LEFT].PushBack({ 133,166,41,42 });
	animAtac[FIXED_ANGLE::LEFT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::LEFT].loop = false;
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 1,541,45,49 });
	animAtac[FIXED_ANGLE::UP_LEFT].PushBack({ 141,395,40,48 });
	animAtac[FIXED_ANGLE::UP_LEFT].speedFactor = 2.0f;
	animAtac[FIXED_ANGLE::UP_LEFT].loop = false;


	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP_RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP_RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP_RIGHT].loop = false;
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 48,543,44,50 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 45,345,46,46 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].PushBack({ 92,394,47,47 });
	animDeath[FIXED_ANGLE::DOWN_RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN_RIGHT].loop = false;
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN_LEFT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN_LEFT].loop = false;
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 181,450,47,48 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 56,41,53,39 });
	animDeath[FIXED_ANGLE::UP_LEFT].PushBack({ 1,209,44,50 });
	animDeath[FIXED_ANGLE::UP_LEFT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP_LEFT].loop = false;
	animDeath[FIXED_ANGLE::UP].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::UP].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::UP].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::UP].loop = false;
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 1,491,44,48 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 93,346,46,46 });
	animDeath[FIXED_ANGLE::DOWN].PushBack({ 183,401,48,47 });
	animDeath[FIXED_ANGLE::DOWN].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::DOWN].loop = false;
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 92,443,47,48 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 1,41,53,39 });
	animDeath[FIXED_ANGLE::RIGHT].PushBack({ 176,166,44,43 });
	animDeath[FIXED_ANGLE::RIGHT].speedFactor = 9.0f;
	animDeath[FIXED_ANGLE::RIGHT].loop = false;
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
