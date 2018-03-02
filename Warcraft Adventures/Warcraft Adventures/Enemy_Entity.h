#ifndef __EnemyEntity_H__
#define __EnemyEntity_H__

#include "Entity.h"

enum ENEMY_STATES {

};

enum ENEMY_ACTIONS {

};

class Enemy_Entity : public DinamicEntity {
public:
	Enemy_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, ENEMY_TYPE character);
	void DoAction(ENEMY_ACTIONS action, FIXED_ANGLE angle);
	void UseConsumable();

	void Start();
	void Finish();
	void Draw();

	void Update();
	void Move();

public:
	ENEMY_STATES state;
	ENEMY_TYPE boss_type;

};

#endif