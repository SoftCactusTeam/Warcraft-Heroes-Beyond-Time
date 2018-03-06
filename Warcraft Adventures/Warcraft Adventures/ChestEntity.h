#ifndef __CHESTENTITY_H__
#define __CHESTENTITY_H__

#include "StaticEntity.h"

class ChestEntiy : public StaticEntity {
public:
	ChestEntiy(iPoint coor, CHEST_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Finish();

private:
	CHEST_TYPE type = CHEST_TYPE::NON_CHEST;
};

#endif