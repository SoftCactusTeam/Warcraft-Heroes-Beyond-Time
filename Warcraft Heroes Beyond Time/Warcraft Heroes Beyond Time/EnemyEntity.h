#ifndef __ENEMYENTITY_H__
#define __ENEMYENTITY_H__

#include "DynamicEntity.h"
#include "Pathfinding.h"
#include "ModuleEntitySystem.h"

#define NUMBER_OF_ORIENTATIONS 8

class EnemyEntity : public DynamicEntity 
{
public:
	EnemyEntity(fPoint coor, ENEMY_TYPE tier, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();

	FIXED_ANGLE LookAtPlayer();
	uint DistanceToPlayer();
public:
	ENEMY_TYPE enemytype = ENEMY_TYPE::NO_TIER;
	EntitySystem::EnemyStats numStats;

protected:
	PathVector pathVector;
};

#endif