#ifndef __ENEMYENTITY_H__
#define __ENEMYENTITY_H__

#include "DynamicEntity.h"

class EnemyEntity : public DynamicEntity {
public:
	EnemyEntity(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update();
	virtual bool Finish();

public:
	ENEMY_TYPE type = ENEMY_TYPE::NON_ENEMY;

};

#endif