#ifndef __Enemy_Archer_H__
#define __Enemy_Archer_H__

#include "EnemyEntity.h"

enum ARCHER_STATE {
	ARCHER_IDLE,
	ARCHER_WALK,
	ARCHER_ATAC,
	ARCHER_CHARGE
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
};

#endif