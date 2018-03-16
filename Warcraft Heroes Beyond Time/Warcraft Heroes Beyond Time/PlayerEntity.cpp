#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"

PlayerEntity::PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start()
{
	anim = &idleDown;
	state = states::PL_IDLE;

	endPos.x = 250.0f;
	endPos.y = 250.0f;

	return true;
}

bool PlayerEntity::Update(float dt) 
{ 
	

	return true; 
}

bool PlayerEntity::Finish() { return true; }

fPoint PlayerEntity::CalculatePosFromBezier(fPoint startPos, fPoint handleA, float t, fPoint handleB, fPoint endPos)
{
	float t2 = pow(t, 2.0f);
	float t3 = pow(t, 3.0f);
	float subT = 1.0f - t;
	float subT2 = pow(subT, 2);
	float subT3 = pow(subT, 3);

	fPoint firstArgument;
	firstArgument.x = subT3 * startPos.x;
	firstArgument.y = subT3 * startPos.y ;

	fPoint secondArgument;
	secondArgument.x = 3.0f * t * subT2 * handleA.x;
	secondArgument.y = 3.0f * t * subT2 * handleA.y;

	fPoint thirdArgument;
	thirdArgument.x = 3.0f * t2 * subT * handleB.x;
	thirdArgument.y = 3.0f * t2 * subT * handleB.y;

	fPoint fourthArgument;
	fourthArgument.x = t3 * endPos.x;
	fourthArgument.y = t3 * endPos.y;

	fPoint res;
	res.x = firstArgument.x + secondArgument.x + thirdArgument.x + fourthArgument.x;
	res.y = firstArgument.y + secondArgument.y + thirdArgument.y + fourthArgument.y;

	return res;
}

void PlayerEntity::PlayerStates(float dt)
{

	// ----- Dash test -----

	/*if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_REPEAT  && !dashEnabled)
	{
		dashEnabled = true;
		startPos = pos;
		endPos.x = pos.x + 30.0f;
		endPos.y = pos.y;
	}

	if (dashEnabled && t <= 1.0f && t >= 0.0f)
	{
		t += 0.05f;
		pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * 250.0f;
		
	}

	if (t >= 1.0f)
	{
		dashEnabled = false;
		t = 0.0f;
	}*/
	

	// -----------------

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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && t == 0.0f)
		{
			startPos = pos;
			last_state = state;
			state = states::PL_DASH;
			animBeforeDash = anim;
			
			break;
		}
	
		break;

	case states::PL_DASH:

		if (t <= 1.0f && t >= 0.0f)
		{
			t += 0.05f;

			if (animBeforeDash == &idleRight || animBeforeDash == &right)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				anim = &dashRight;
				break;
			}
			else if (animBeforeDash == &idleLeft || animBeforeDash == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				anim = &dashLeft;
				break;
			}
			else if (animBeforeDash == &idleUp || animBeforeDash == &up)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashUp;
				break;
			}
			else if (animBeforeDash == &idleDown || animBeforeDash == &down)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashDown;
				break;
			}
			else if (animBeforeDash == &idleUpRight || animBeforeDash ==&upRight)
			{
				pos.x = startPos.x + 0.75f * CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				pos.y = startPos.y - 0.75f * CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;

				anim = &dashUpRight;
				break;
			}
		}
		else
		{
			state = states::PL_IDLE;
			t = 0.0f;

			if (anim == &dashRight)
				anim = &idleRight;
			else if (anim == &dashLeft)
				anim = &idleLeft;
			else if (anim == &dashUp)
				anim = &idleUp;
			else if (anim == &dashDown)
				anim = &idleDown;
			else if (anim == &dashUpRight)
				anim = &idleUpRight;
			else if (anim == &dashUpLeft)
				anim = &idleUpLeft;
			else if (anim == &dashDownRight)
				anim = &idleDownRight;
			else if (anim == &dashDownLeft)
				anim = &idleDownLeft;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;

			break;
		}
		break;

	case states::PL_DASH:

		if (t <= 1.0f && t >= 0.0f)
		{
			t += 0.05f;

			if (animBeforeDash == &idleRight || animBeforeDash == &right)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				anim = &dashRight;
				break;
			}
			else if (animBeforeDash == &idleLeft || animBeforeDash == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				anim = &dashLeft;
				break;
			}
			else if (animBeforeDash == &idleUp || animBeforeDash == &up)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashUp;
				break;
			}
			else if (animBeforeDash == &idleDown || animBeforeDash == &down)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashDown;
				break;
			}
			else if (animBeforeDash == &idleUpRight || last_state == states::PL_UP_RIGHT)
			{
				pos.x = startPos.x + 0.75f * CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).x * endPos.x;
				pos.y = startPos.y - 0.75f * CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashUpRight;
				break;
			}
		}
		else
		{
			state = states::PL_IDLE;
			t = 0.0f;

			if (anim == &dashRight)
				anim = &idleRight;
			else if (anim == &dashLeft)
				anim = &idleLeft;
			else if (anim == &dashUp)
				anim = &idleUp;
			else if (anim == &dashDown)
				anim = &idleDown;
			else if (anim == &dashUpRight)
				anim = &idleUpRight;
			else if (anim == &dashUpLeft)
				anim = &idleUpLeft;
			else if (anim == &dashDownRight)
				anim = &idleDownRight;
			else if (anim == &dashDownLeft)
				anim = &idleDownLeft;
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
		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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