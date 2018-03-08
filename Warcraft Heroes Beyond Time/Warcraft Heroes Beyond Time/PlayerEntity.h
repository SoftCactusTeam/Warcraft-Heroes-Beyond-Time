#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "DynamicEntity.h"

class PlayerEntity : public DynamicEntity {
protected:
	PLAYER_TYPE type = PLAYER_TYPE::NON_PLAYER;

public:
	PlayerEntity(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool Finish();
};

#endif