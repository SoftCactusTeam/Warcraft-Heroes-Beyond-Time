#ifndef __ENEMY_FOOTMAN_H__
#define __ENEMY_FOOTMAN_H__

#include "EnemyEntity.h"

enum FOOTMAN_STATE{
	FOOTMAN_IDLE,
	FOOTMAN_WALK,
	FOOTMAN_ATAC,
	FOOTMAN_CHARGE,
	FOOTMAN_DEFENSE
};

class Enemy_Footman : public EnemyEntity
{
public:
	Enemy_Footman(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

	void ChargeAnimations();

	void initIdle();
	void initWalk();
	void initAtac();
	void initCharge();
	void initDefense();

	void doIdle();
	void doWalk();
	void doAtac();
	void doCharge();
	void doDefense();

public:
	FOOTMAN_STATE state;
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];
	Animation animPreCharge[NUMBER_OF_ORIENTATIONS];
	Animation animCharge[NUMBER_OF_ORIENTATIONS];
	Animation animPostCharge[NUMBER_OF_ORIENTATIONS];

	// Charge variables
	fPoint		chargeMovement;
	int			chargeCooldown = 0;
	int			chargeTime = 0;
	FIXED_ANGLE saveFirstAngle;
	// Invulenarbility variables
	bool		defensed = false;
	int			defenseCooldown = 0;
};

#endif