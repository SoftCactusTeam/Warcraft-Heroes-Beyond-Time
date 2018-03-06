#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "DynamicEntity.h"

class PlayerEntity : public DynamicEntity {
public:
	PlayerEntity(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update();
	bool Finish();

private:
	PLAYER_TYPE type = PLAYER_TYPE::NON_PLAYER;
};

#endif