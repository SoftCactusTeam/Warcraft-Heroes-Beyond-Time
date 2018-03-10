#include "App.h"
#include "Thrall.h"
#include "ModuleInput.h"

Thrall::Thrall(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture) 
{
	idle.PushBack({ 282,16,36,49 });
	idle.speed = 1.0f;
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