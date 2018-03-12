#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"

Thrall::Thrall(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture) 
{
	idleDown.PushBack({ 0,188,55,47 });
	idleDown.PushBack({ 55,188,55,47 });
	idleDown.PushBack({ 110,188,55,47 });
	idleDown.PushBack({ 165,188,55,47 });
	idleDown.speed = 0.2f;

	idleUp.PushBack({ 0,0,55,47 });
	idleUp.PushBack({ 55,0,55,47 });
	idleUp.PushBack({ 110,0,55,47 });
	idleUp.PushBack({ 165,0,55,47 });
	idleUp.speed = 0.2f;

	idleLeft.PushBack({ 0,282,55,47 });
	idleLeft.PushBack({ 55,282,55,47 });
	idleLeft.PushBack({ 110,282,55,47 });
	idleLeft.PushBack({ 165,282,55,47 });
	idleLeft.speed = 0.2f;

	idleRight.PushBack({ 0,94,55,47 });
	idleRight.PushBack({ 55,94,55,47 });
	idleRight.PushBack({ 110,94,55,47 });
	idleRight.PushBack({ 165,94,55,47 });
	idleRight.speed = 0.2f;

	idleUpRight.PushBack({ 0,47,55,47 });
	idleUpRight.PushBack({ 55,47,55,47 });
	idleUpRight.PushBack({ 110,47,55,47 });
	idleUpRight.PushBack({ 165,47,55,47 });
	idleUpRight.speed = 0.2f;

	idleUpLeft.PushBack({ 0,329,55,47 });
	idleUpLeft.PushBack({ 55,329,55,47 });
	idleUpLeft.PushBack({ 110,329,55,47 });
	idleUpLeft.PushBack({ 165,329,55,47 });
	idleUpLeft.speed = 0.2f;

	idleDownRight.PushBack({ 0,141,55,47 });
	idleDownRight.PushBack({ 55,141,55,47 });
	idleDownRight.PushBack({ 110,141,55,47 });
	idleDownRight.PushBack({ 165,141,55,47 });
	idleDownRight.speed = 0.2f;

	idleDownLeft.PushBack({ 0,235,55,47 });
	idleDownLeft.PushBack({ 55,235,55,47 });
	idleDownLeft.PushBack({ 110,235,55,47 });
	idleDownLeft.PushBack({ 165,235,55,47 });
	idleDownLeft.speed = 0.2f;

	down.PushBack({ 0,564,55,47 });
	down.PushBack({ 55,564,55,47 });
	down.PushBack({ 110,564,55,47 });
	down.PushBack({ 165,564,55,47 });
	down.speed = 0.2f;

	up.PushBack({ 0,376,55,47 });
	up.PushBack({ 55,376,55,47 });
	up.PushBack({ 110,376,55,47 });
	up.PushBack({ 165,376,55,47 });
	up.speed = 0.2f;

	left.PushBack({ 0,658,55,47 });
	left.PushBack({ 55,658,55,47 });
	left.PushBack({ 110,658,55,47 });
	left.PushBack({ 165,658,55,47 });
	left.speed = 0.2f;

	right.PushBack({ 0,470,55,47 });
	right.PushBack({ 55,470,55,47 });
	right.PushBack({ 110,470,55,47 });
	right.PushBack({ 165,470,55,47 });
	right.speed = 0.2f;

	upRight.PushBack({ 0,423,55,47 });
	upRight.PushBack({ 55,423,55,47 });
	upRight.PushBack({ 110,423,55,47 });
	upRight.PushBack({ 165,423,55,47 });
	upRight.speed = 0.2f;

	upLeft.PushBack({ 0,705,55,47 });
	upLeft.PushBack({ 55,705,55,47 });
	upLeft.PushBack({ 110,705,55,47 });
	upLeft.PushBack({ 165,705,55,47 });
	upLeft.speed = 0.2f;

	downRight.PushBack({ 0,517,55,47 });
	downRight.PushBack({ 55,517,55,47 });
	downRight.PushBack({ 110,517,55,47 });
	downRight.PushBack({ 165,517,55,47 });
	downRight.speed = 0.2f;

	downLeft.PushBack({ 0,611,55,47 });
	downLeft.PushBack({ 55,611,55,47 });
	downLeft.PushBack({ 110,611,55,47 });
	downLeft.PushBack({ 165,611,55,47 });
	downLeft.speed = 0.2f;
}

bool Thrall::Update(float dt)
{
	PlayerStates(dt);

	return true;
}

bool Thrall::Finish()
{	
	return true;
}