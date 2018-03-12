#include "EnemyEntity.h"
#include "Application.h"
#include "ModuleEntitySystem.h"

EnemyEntity::EnemyEntity(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : DynamicEntity(coor, texture), type(type) {}

bool EnemyEntity::Start() { return true; }

bool EnemyEntity::Update(float dt) { return true; }

bool EnemyEntity::Finish() { return true; }

void EnemyEntity::LookAtPlayer()
{
	switch (CaculateAngle(App->entities->actualPlayer->pos))
	{
	case FIXED_ANGLE::UP:
		orientation = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::UP_RIGHT:
		orientation = FIXED_ANGLE::UP_RIGHT;
		break;
	case FIXED_ANGLE::RIGHT:
		orientation = FIXED_ANGLE::RIGHT;
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		orientation = FIXED_ANGLE::DOWN_RIGHT;
		break;
	case FIXED_ANGLE::DOWN:
		orientation = FIXED_ANGLE::DOWN;
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		orientation = FIXED_ANGLE::DOWN_LEFT;
		break;
	case FIXED_ANGLE::LEFT:
		orientation = FIXED_ANGLE::LEFT;
		break;
	case FIXED_ANGLE::UP_LEFT:
		orientation = FIXED_ANGLE::UP_LEFT;
		break;
	}
}

uint EnemyEntity::DistanceToPlayer()
{
	return DistanceToObejective(App->entities->actualPlayer->pos);
}