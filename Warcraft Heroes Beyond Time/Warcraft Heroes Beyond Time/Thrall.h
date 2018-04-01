#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"

class Thrall : public PlayerEntity {
private:
public:
	Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Update(float dt);
	bool Finish();

	void Collision(COLLIDER_TYPE type);
};

#endif