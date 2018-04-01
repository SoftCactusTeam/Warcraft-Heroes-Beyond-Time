#ifndef __ENEMY_FOOTMAN_H__
#define __ENEMY_FOOTMAN_H__

#include "EnemyEntity.h"

enum FOOTMAN_STATE{
	FOOTMAN_IDLE,
	FOOTMAN_WALK,
	FOOTMAN_ATAC,
	FOOTMAN_CHARGE
};

class Enemy_Footman : public EnemyEntity
{
public:
	Enemy_Footman(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

	void ChargeAnimations();

	void doIdle();
	void doWalk();
	void doAtac();
	void doCharge();

public:
	FOOTMAN_STATE state;
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];
	Animation animCharge[NUMBER_OF_ORIENTATIONS];

	// Charge variables
	fPoint		chargeMovement;
	uint		chargeTime = 0;
};

#endif