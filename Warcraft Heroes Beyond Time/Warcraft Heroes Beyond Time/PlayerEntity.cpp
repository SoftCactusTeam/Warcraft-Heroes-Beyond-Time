#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"

PlayerEntity::PlayerEntity(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start()
{
	anim = &idleDown;
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
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT  && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		break;
	case states::PL_UP:
		pos.y -= 5;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleUp;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		break;

	case states::PL_DOWN:
		pos.y += 5;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleDown;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;

	case states::PL_LEFT:
		pos.x -= 5;
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;
	case states::PL_RIGHT:
		pos.x += 5;
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		break;

	case states::PL_UP_LEFT:
		pos.x -= 5;
		pos.y -= 5;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleUpLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{	
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{			
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_UP_RIGHT:
		pos.x += 5;
		pos.y -= 5;
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{			
			state = states::PL_IDLE;
			anim = &idleUpRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{			
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_DOWN_LEFT:
		pos.x -= 5;
		pos.y += 5;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{		
			state = states::PL_IDLE;
			anim = &idleDownLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{		
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{		
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;

	case states::PL_DOWN_RIGHT:
		pos.x += 5;
		pos.y += 5;
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{			
			state = states::PL_IDLE;
			anim = &idleDownRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{			
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;
	}

}