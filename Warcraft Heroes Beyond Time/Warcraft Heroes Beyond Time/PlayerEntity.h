#ifndef __PLAYERENTITY_H__
#define __PLAYERENTITY_H__

#include "DynamicEntity.h"

class PlayerEntity : public DynamicEntity {
protected:
	PLAYER_TYPE type = PLAYER_TYPE::NON_PLAYER;
	Animation idleDown, idleUp, idleLeft, idleRight, idleUpRight, idleUpLeft, idleDownRight, idleDownLeft;
	Animation up, down, left, right, upLeft, upRight, downLeft, downRight;
	float speed = 250.0f;

	enum class states
	{
		PL_NON_STATE,
		PL_IDLE,
		PL_UP,
		PL_DOWN,
		PL_LEFT,
		PL_RIGHT,
		PL_UP_RIGHT,
		PL_UP_LEFT,
		PL_DOWN_RIGHT,
		PL_DOWN_LEFT
	} state;

public:
	PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture);

	virtual bool Start();
	virtual bool Update(float dt);
	void PlayerStates(float dt);
	void KeyboardStates(float dt);
	void JoyconStates(float dt);
	virtual bool Finish();
};

#endif