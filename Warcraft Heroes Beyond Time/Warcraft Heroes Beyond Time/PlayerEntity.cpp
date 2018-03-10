#include "App.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"

PlayerEntity::PlayerEntity(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start()
{
	anim = &idle;
	state = states::PL_IDLE;

	return true;
}

bool PlayerEntity::Update(float dt) { return true; }

bool PlayerEntity::Finish() { return true; }

void PlayerEntity::PlayerStates(float dt)
{
	switch (state)
	{
	case states::PL_IDLE:
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_LEFT;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_RIGHT;
			break;
		}
		break;
	case states::PL_UP:
		pos.y -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &idle;
			break;
		}
		break;

	case states::PL_DOWN:
		pos.y += 5;
		if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &idle;
			break;
		}
		break;

	case states::PL_LEFT:
		pos.x -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &idle;
			break;
		}
		break;
	case states::PL_RIGHT:
		pos.x += 5;
		if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &idle;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &idle;
			break;
		}
		break;

	case states::PL_UP_LEFT:
		pos.x -= 5;
		pos.y -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP && Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_IDLE;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::PL_LEFT;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_UP;
			break;
		}
		break;

	case states::PL_UP_RIGHT:
		pos.x += 5;
		pos.y -= 5;
		if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP && Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_IDLE;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::PL_RIGHT;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_UP;
			break;
		}
		break;

	case states::PL_DOWN_LEFT:
		pos.x -= 5;
		pos.y += 5;
		if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_IDLE;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::PL_LEFT;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_DOWN;
			break;
		}
		break;

	case states::PL_DOWN_RIGHT:
		pos.x += 5;
		pos.y += 5;
		if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_IDLE;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::PL_RIGHT;
			break;
		}
		else if (Application->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_DOWN;
			break;
		}
		break;
	}

}