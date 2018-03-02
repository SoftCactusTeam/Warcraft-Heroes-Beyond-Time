#ifndef __BossEntity_H__
#define __BossEntity_H__

#include "Entity.h"

enum BOSS_STATES {

};

enum BOSS_ACTIONS {

};

class Boss_Entity : public DinamicEntity {
public:
	Boss_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, BOSS_TYPE character);
	void DoAction(BOSS_ACTIONS action, FIXED_ANGLE angle);
	void UseConsumable();

	void Start();
	void Finish();
	void Draw();

	void Update();
	void Move();

public:
	BOSS_STATES state;
	BOSS_TYPE boss_type;

};

#endif