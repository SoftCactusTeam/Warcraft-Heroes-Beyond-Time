#ifndef __PlayerEntity_H__
#define __PlayerEntity_H__

#include "Entity.h"

enum PLAYER_STATES {

};

enum PLAYER_ACTIONS {

};

class PlayerPJ_Entity : public DinamicEntity {
public:
	PlayerPJ_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, PLAYER_TYPE character);
	void DoAction(PLAYER_ACTIONS action, FIXED_ANGLE angle);
	void UseConsumable();

	void Start();
	void Finish();

	void Update();
	void Move();

public:
	PLAYER_STATES state;
	CONSUMABLE_TYPE consumable;
	PLAYER_TYPE characterPJ_type;
	
	int skillCooldown;
};

#endif