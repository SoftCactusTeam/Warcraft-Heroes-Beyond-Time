#include "App.h"
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