#include "App.h"
#include "Thrall.h"
#include "ModuleInput.h"

Thrall::Thrall(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture) 
{
	
}

bool Thrall::Start()
{
	rectToPrint = { 282,16,36,49 };
	state = states::idle;
	return true;
}

bool Thrall::Update(float dt)
{
	switch (state)
	{
	case states::idle:
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::up;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::down;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::left;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::right;
			break;
		}
		break;
	case states::up:
		pos.y -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::idle;
			rectToPrint = { 282,16,36,49 };
			break;
		}

		break;
	case states::down:
		pos.y += 5;
		if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::idle;
			rectToPrint = { 282,16,36,49 };
			break;
		}
		break;
	case states::left:
		pos.x -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::idle;
			rectToPrint = { 282,16,36,49 };
			break;
		}
		break;
	case states::right:
		pos.x += 5;
		if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::idle;
			rectToPrint = { 282,16,36,49 };
			break;
		}
		break;
	}

	return true;
}

bool Thrall::Finish()
{	
	return true;
}