#ifndef __ConsumableEntity_H__
#define __ConsumableEntity_H__

#include "Entity.h"

class Consumable_Entity : public StaticEntity {
public:
	Consumable_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, CONSUMABLE_TYPE consumableType);

	void Start();
	void Finish();

	void Interaction();

public:
	CONSUMABLE_TYPE consumableType;
};

#endif