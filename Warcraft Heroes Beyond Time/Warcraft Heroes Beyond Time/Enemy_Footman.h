#ifndef __ENEMY_FOOTMAN_H__
#define __ENEMY_FOOTMAN_H__

#include "EnemyEntity.h"

class Enemy_Footman : public EnemyEntity
{
	Enemy_Footman(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update();
	bool Finish();


};

#endif