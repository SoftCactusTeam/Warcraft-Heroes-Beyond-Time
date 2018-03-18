#ifndef __ENEMYENTITY_H__
#define __ENEMYENTITY_H__

#include "DynamicEntity.h"

class EnemyEntity : public DynamicEntity {
public:
	EnemyEntity(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	bool Start();
	bool Update();
	bool Finish();

public:
	ENEMY_TYPE type = ENEMY_TYPE::NON_ENEMY;

};

#endif