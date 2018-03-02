#ifndef __ConsumableEntity_H__
#define __ConsumableEntity_H__

#include "Entity.h"

class Consumable_Entity : public StaticEntity {
public:
	Consumable_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture);

	void Start();
	void Finish();
	void Draw();

	void Interaction();
};

#endif