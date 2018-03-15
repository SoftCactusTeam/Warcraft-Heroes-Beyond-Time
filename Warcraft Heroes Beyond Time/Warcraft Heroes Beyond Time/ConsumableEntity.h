#ifndef __CONSUMABLEENTITY_H__
#define __CONSUMABLEENTITY_H__

#include "StaticEntity.h"

class ConsumableEntity : public StaticEntity {
public:
	ConsumableEntity(fPoint coor, CONSUMABLE_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Finish();

private:
	CONSUMABLE_TYPE type = CONSUMABLE_TYPE::NON_CONSUMABLE;
};

#endif