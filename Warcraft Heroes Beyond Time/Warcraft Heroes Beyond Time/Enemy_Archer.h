#ifndef __Enemy_Archer_H__
#define __Enemy_Archer_H__

#include "EnemyEntity.h"

#define ARROW_DEAD_TIMER 2000

enum ARCHER_STATE {
	ARCHER_IDLE,
	ARCHER_WALK,
	ARCHER_ATAC,
	ARCHER_CHARGE
};

class Enemy_Archer_Arrow {
public:
	Enemy_Archer_Arrow(fPoint coor, SDL_Texture* texture, fPoint direction, FIXED_ANGLE angle,int deadTimer = ARROW_DEAD_TIMER);

	void Update();
	void Finish();

public:
	fPoint			pos;
	fPoint			direction;
	SDL_Texture*	texture;
	SDL_Rect		rect[NUMBER_OF_ORIENTATIONS];
	FIXED_ANGLE		angle;
	int				deadTimer;

};

class Enemy_Archer : public EnemyEntity
{
public:
	Enemy_Archer(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();

	void ChargeAnimations();

	void doIdle();
	void doWalk();
	void doAtac();

public:
	ARCHER_STATE state;
	Animation animIdle[NUMBER_OF_ORIENTATIONS];
	Animation animWalk[NUMBER_OF_ORIENTATIONS];
	Animation animAtac[NUMBER_OF_ORIENTATIONS];
	Animation animDeath[NUMBER_OF_ORIENTATIONS];
};

#endif