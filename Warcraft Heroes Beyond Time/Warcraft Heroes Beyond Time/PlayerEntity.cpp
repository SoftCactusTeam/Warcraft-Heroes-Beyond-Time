#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"

PlayerEntity::PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

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
	if (move)
	{
		if (App->input->IsKeyboardAvailable())
			KeyboardStates(dt);
		else
			JoyconStates(dt);
	}
	else
	{
		switch (state)
		{
		case states::PL_DOWN:
			anim = &idleDown;
			state = states::PL_IDLE;
			break;
		case states::PL_DOWN_LEFT:
			anim = &idleDownLeft;
			state = states::PL_IDLE;
			break;
		case states::PL_LEFT:
			anim = &idleLeft;
			state = states::PL_IDLE;
			break;
		case states::PL_UP_LEFT:
			anim = &idleUpLeft;
			state = states::PL_IDLE;
			break;
		case states::PL_UP:
			anim = &idleUp;
			state = states::PL_IDLE;
			break;
		case states::PL_UP_RIGHT:
			anim = &idleUpRight;
			state = states::PL_IDLE;
			break;
		case states::PL_RIGHT:
			anim = &idleRight;
			state = states::PL_IDLE;
			break;
		case states::PL_DOWN_RIGHT:
			anim = &idleDownRight;
			state = states::PL_IDLE;
			break;
		}
	}
}

void PlayerEntity::KeyboardStates(float dt)
{
	switch (state)
	{
	case states::PL_IDLE:
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT  && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		break;

	case states::PL_UP:
		pos.y -= speed * dt;
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_UP))
		{
			state = states::PL_IDLE;
			anim = &idleUp;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		break;

	case states::PL_DOWN:
		pos.y += speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleDown;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;

	case states::PL_LEFT:
		pos.x -= speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;

	case states::PL_RIGHT:
		pos.x += speed * dt;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		break;

	case states::PL_UP_LEFT:
		pos.x -= speed * 0.75f * dt;
		pos.y -= speed * 0.75f * dt;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleUpLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_UP_RIGHT:
		pos.x += speed * 0.75f * dt;
		pos.y -= speed * 0.75f * dt;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleUpRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_DOWN_LEFT:
		pos.x -= speed * 0.75f * dt;
		pos.y += speed * 0.75f * dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleDownLeft;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;

	case states::PL_DOWN_RIGHT:
		pos.x += speed * 0.75f * dt;
		pos.y += speed * 0.75f * dt;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = states::PL_IDLE;
			anim = &idleDownRight;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;
	}
}

void PlayerEntity::JoyconStates(float dt)
{
	switch (state)
	{
	case states::PL_IDLE:
		if (App->input->GetYAxis() < 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		else if (App->input->GetYAxis() < 0)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		else if (App->input->GetYAxis() > 0)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		else if (App->input->GetXAxis() < 0)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetXAxis() > 0)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		break;

	case states::PL_UP:
		pos.y -= speed * dt;
		if (App->input->GetYAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleUp;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		break;

	case states::PL_DOWN:
		pos.y += speed * dt;
		if (App->input->GetYAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleDown;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;

	case states::PL_LEFT:
		pos.x -= speed * dt;
		if (App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleLeft;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_UP_LEFT;
			anim = &upLeft;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() < 0)
		{
			state = states::PL_DOWN_LEFT;
			anim = &downLeft;
			break;
		}
		break;

	case states::PL_RIGHT:
		pos.x += speed * dt;
		if (App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleRight;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_UP_RIGHT;
			anim = &upRight;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() > 0)
		{
			state = states::PL_DOWN_RIGHT;
			anim = &downRight;
			break;
		}
		break;

	case states::PL_UP_LEFT:
		pos.x -= speed * 0.75f * dt;
		pos.y -= speed * 0.75f * dt;
		if (App->input->GetYAxis() == 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleUpLeft;
			break;
		}
		else if (App->input->GetXAxis() < 0 && App->input->GetYAxis() == 0)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_UP_RIGHT:
		pos.x += speed * 0.75f * dt;
		pos.y -= speed * 0.75f * dt;
		if (App->input->GetYAxis() == 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleUpRight;
			break;
		}
		else if (App->input->GetXAxis() > 0 && App->input->GetYAxis() == 0)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetYAxis() < 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_UP;
			anim = &up;
			break;
		}
		break;

	case states::PL_DOWN_LEFT:
		pos.x -= speed * 0.75f * dt;
		pos.y += speed * 0.75f * dt;
		if (App->input->GetYAxis() == 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleDownLeft;
			break;
		}
		else if (App->input->GetXAxis() < 0 && App->input->GetYAxis() == 0)
		{
			state = states::PL_LEFT;
			anim = &left;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;

	case states::PL_DOWN_RIGHT:
		pos.x += speed * 0.75f * dt;
		pos.y += speed * 0.75f * dt;
		if (App->input->GetYAxis() == 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_IDLE;
			anim = &idleDownRight;
			break;
		}
		else if (App->input->GetXAxis() > 0 && App->input->GetYAxis() == 0)
		{
			state = states::PL_RIGHT;
			anim = &right;
			break;
		}
		else if (App->input->GetYAxis() > 0 && App->input->GetXAxis() == 0)
		{
			state = states::PL_DOWN;
			anim = &down;
			break;
		}
		break;
	}
}

void PlayerEntity::Walk(bool can)
{
	this->move = can;
}