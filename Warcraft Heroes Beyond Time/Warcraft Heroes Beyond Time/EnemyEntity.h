#ifndef __ENEMYENTITY_H__
#define __ENEMYENTITY_H__

#include "DynamicEntity.h"

#define NUMBER_OF_ORIENTATIONS 8

class EnemyEntity : public DynamicEntity {
public:
	EnemyEntity(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();

	FIXED_ANGLE LookAtPlayer();
	uint DistanceToPlayer();
public:
	ENEMY_TYPE type = ENEMY_TYPE::NON_ENEMY;

protected:
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];

};

#endif