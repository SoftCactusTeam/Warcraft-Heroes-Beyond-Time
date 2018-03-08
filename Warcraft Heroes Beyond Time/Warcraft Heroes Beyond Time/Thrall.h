#ifndef __THRALL_H__
#define __THRALL_H__

#include "PlayerEntity.h"

class Thrall : public PlayerEntity {
private:
	enum class states
	{
		non_state,
		idle,
		up,
		down,
		left,
		right
	} state;
public:
	Thrall(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	bool Start();
	bool Update(float dt);
	bool Finish();
};

#endif