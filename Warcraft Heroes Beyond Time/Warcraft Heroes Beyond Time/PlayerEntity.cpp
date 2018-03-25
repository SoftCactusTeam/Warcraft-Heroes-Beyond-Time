#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleMapGenerator.h"
#include "ModuleEntitySystem.h"

PlayerEntity::PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start()
{
	anim = &idleDown;
	state = states::PL_IDLE;

	InitCulling();

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
	if (move)
	{
		if (App->input->IsKeyboardAvailable())
			KeyboardStates(dt);
		else
			JoyconStates(dt);

		CheckMapLimits();
		CheckCulling();
	}
	else
	{
		CheckIddleStates();
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
			state = states::PL_DASH;
			animBeforeDash = anim;
			
			break;
		}
	
		break;

	case states::PL_DASH:

		if (t <= 1.0f && t >= 0.0f)
		{
			if (animBeforeDash == &idleRight || animBeforeDash == &right)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				
				anim = &dashRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleLeft || animBeforeDash == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				
				anim = &dashLeft;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleUp || animBeforeDash == &up)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashUp;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleDown || animBeforeDash == &down)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashDown;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleUpRight || animBeforeDash ==&upRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });
					
				pos.x = startPos.x + 250.0f * 0.75f * bezierPoint.y;
				pos.y = startPos.y - 250.0f * 0.75f * bezierPoint.y;

				anim = &dashUpRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleDownRight || animBeforeDash == &downRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + 250.0f * 0.75f * bezierPoint.y;
				pos.y = startPos.y + 250.0f * 0.75f * bezierPoint.y;

				anim = &dashDownRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleDownLeft || animBeforeDash == &downLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x - 250.0f * 0.75f * bezierPoint.y;
				pos.y = startPos.y + 250.0f * 0.75f * bezierPoint.y;

				anim = &dashDownLeft;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleUpLeft || animBeforeDash == &upLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x - 250.0f * 0.75f * bezierPoint.y;
				pos.y = startPos.y - 250.0f * 0.75f * bezierPoint.y;

				anim = &dashUpLeft;
				float x = 0.05f / dt;
				t += (x * dt);
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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
		if (App->input->GetXAxis() != 0 || App->input->GetYAxis() != 0)
		{
			state = states::PL_MOVE;
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
			if (animBeforeDash == &idleRight)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * 250.0f;
				anim = &dashRight;
				
			}
			else if (animBeforeDash == &idleLeft)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashLeft; //
			}
			else if (animBeforeDash == &idleUp)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashUp; //
			}
			else if (animBeforeDash == &idleDown)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashDown;
			}
			else if (animBeforeDash == &idleUpRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(315.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(315.0f));

				anim = &dashUpRight;
			}
			else if (animBeforeDash == &idleUpLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(225.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(225.0f));

				anim = &dashUpLeft; //
			}
			else if (animBeforeDash == &idleDownRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(45.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(45.0f));

				anim = &dashDownRight;
			}
			else if (animBeforeDash == &idleDownLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(135.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(135.0f));

				anim = &dashDownLeft;
			}
			else
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance * cos(DEG_2_RAD(angle));
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance * sin(DEG_2_RAD(angle));
			
				anim = GetAnimFromAngle(angle, true);
			}
			
			float x = 0.05f / dt;
			t += (x * dt);

		}
		else
		{
			if (App->input->InsideDeadZone())
			{
				state = states::PL_IDLE;

				if (anim == &dashRight)
					anim = &idleRight;
				else if (anim == &dashDown)
					anim = &idleDown;
				else if (anim == &dashUpRight)
					anim = &idleUpRight;
				else if (anim == &dashDownLeft)
					anim = &idleDownLeft;
				else if (anim == &dashDownRight)
					anim = &idleDownRight;
				else if (anim == &dashUpRight)
					anim = &idleUpRight;
				else if (anim == &dashLeft)
					anim = &idleLeft;
				else if (anim == &dashUpLeft)
					anim = &idleUpLeft;
				else if (anim == &dashUp)
					anim = &idleUp;
			}
			else
			{
				state = states::PL_MOVE;
				anim = animBeforeDash;
			}
				
			animBeforeDash = nullptr;
			t = 0.0f;
		}

		break;

		case states::PL_MOVE:
			
			if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f)
			{
				animBeforeDash = anim;
				startPos = pos;
				state = states::PL_DASH;

				float X = App->input->GetXAxis() / MAX_JAXIS_VALUE;
				float Y = App->input->GetYAxis() / MAX_JAXIS_VALUE;

				angle = App->input->GetAngleFromAxis();

				break;
			}

			float X = App->input->GetXAxis() / MAX_JAXIS_VALUE;
			float Y = App->input->GetYAxis() / MAX_JAXIS_VALUE;

			pos.x += X * speed * dt;
			pos.y += Y * speed * dt;

			angle = App->input->GetAngleFromAxis();

			Animation* tmpAnim = GetAnimFromAngle(angle);

			if (tmpAnim != nullptr)
				anim = tmpAnim;

			if (App->input->GetXAxis() == 0 && App->input->GetYAxis() == 0)
			{
				if (anim == &up)
					anim = &idleUp;
				if (anim == &down)
					anim = &idleDown;
				else if (anim == &right)
					anim = &idleRight;
				else if (anim == &left)
					anim = &idleLeft;
				else if (anim == &upRight)
					anim = &idleUpRight;
				else if (anim == &upLeft)
					anim = &idleUpLeft;
				else if (anim == &downRight)
					anim = &idleDownRight;
				else if (anim == &downLeft)
					anim = &idleDownLeft;

				state = states::PL_IDLE;
				break;
			}

		break;
	}
}

void PlayerEntity::CheckIddleStates()
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

Animation* PlayerEntity::GetAnimFromAngle(float angle, bool dashOn)
{
	Animation* animToReturn = nullptr;

	if (angle >= 247.5f && angle < 292.5f) // to change
	{
		if (!dashOn)
			animToReturn = &up;
		else
			animToReturn = &dashUp;
	}
		
	else if (angle >= 67.5f && angle < 112.5f)
	{
		if (!dashOn)
			animToReturn = &down;
		else
			animToReturn = &dashDown;
	}

	else if (((angle >= 337.5f && angle < 360.0f) || (angle >= 0 && angle < 22.5f) && !App->input->InsideDeadZone()))
	{
		if (!dashOn)
			animToReturn = &right;
		else
			animToReturn = &dashRight;
	}

	else if (angle >= 157.5f && angle < 202.5f) // to change
	{
		if (!dashOn)
			animToReturn = &left;
		else
			animToReturn = &dashLeft;
	}

	else if (angle >= 292.5f && angle < 337.5f)
	{
		if (!dashOn)
			animToReturn = &upRight;
		else
			animToReturn = &dashUpRight;
	}

	else if (angle >= 202.5f && angle < 247.5f) // to change
	{
		if (!dashOn)
			animToReturn = &upLeft;
		else
			animToReturn = &dashUpLeft;
	}

	else if (angle >= 112.5f && angle < 157.5f)
	{
		if (!dashOn)
			animToReturn = &downLeft;
		else
			animToReturn = &dashDownLeft;
	}

	else if (angle >= 22.5f && angle < 67.5f)
	{
		if (!dashOn)
			animToReturn = &downRight;
		else
			animToReturn = &dashDownRight;
	}
	else
	{
		if (dashOn)
			animToReturn = &dashRight;
	}

	return animToReturn;
}

bool PlayerEntity::IsPlayerMoving()
{
	return state == states::PL_MOVE;
}

void PlayerEntity::Walk(bool can)
{
	this->move = can;
}

void PlayerEntity::InitCulling()
{
	if (this == App->entities->player)
	{
		SDL_Rect currRect = anim->GetCurrentRect();
		App->render->fcamerax = this->pos.x + App->render->camera.w / 4 + currRect.w / 2;
		App->render->fcameray = this->pos.y + App->render->camera.h / 4 - currRect.h;

		freeZonex = pos.x - 55 / 2;
		freeZoney = pos.y - 55 / 2;

		freeZone.x = pos.x - 55;
		freeZone.y = pos.y - 55;
		freeZone.w = 55 / 2 * 2 + 55;
		freeZone.h = 55 / 2 * 2 + 47;
	}
}

void PlayerEntity::CheckCulling()
{
	if (this == App->entities->player)
	{
		SDL_Rect currentRect = anim->GetCurrentRect();
		if (freeZonex > this->pos.x)
		{
			App->render->fcamerax += freeZonex - pos.x;
			freeZonex = this->pos.x;
		}

		else if (freeZonex + freeZone.w < pos.x + currentRect.w)
		{
			App->render->fcamerax -= (pos.x + currentRect.w) - (freeZonex + freeZone.w);
			freeZonex = (this->pos.x + currentRect.w) - freeZone.w;
		}

		if (freeZoney > pos.y)
		{
			App->render->fcameray += freeZoney - pos.y;
			freeZoney = pos.y;
		}
		else if (freeZoney + freeZone.h < pos.y + currentRect.h)
		{
			App->render->fcameray -= (pos.y + currentRect.h) - (freeZoney + freeZone.h);
			freeZoney = pos.y + currentRect.h - freeZone.h;
		}


		freeZone.x = (int)freeZonex;
		freeZone.y = (int)freeZoney;

		//Uncomment line below to see the freeZone.
		//App->render->DrawQuad(freeZone, 255, 0, 0, 50, true, true);
	}
	
}

void PlayerEntity::CheckMapLimits()
{
	uint w, h;
	App->map->getSize(w, h);

	if (w != 0 && h != 0)
	{
		if (pos.x < 0)
			pos.x = 0;

		else if (pos.x + 55 > w * 48)
		{
			pos.x = w * 48 - 55;
		}

		if (pos.y < 0)
			pos.y = 0;

		else if (pos.y + 47 > h * 48)
		{
			pos.y = h * 48 - 47;
		}
	}	
}

void PlayerEntity::AddItem(Item item)
{
	itemsActive.push_back(item);
}

void PlayerEntity::IterateItems(ItemFunctions nameFunction)
{
	std::list<Item>::iterator it = itemsActive.begin();

	for (; it != itemsActive.end(); ++it)
	{
		switch (nameFunction)
		{
		case ItemFunctions::GetItem:
			it->GetItem();
			break;

		case ItemFunctions::UpdateLogic:
			it->UpdateLogic();
			break;

		case ItemFunctions::ByeByeItem:
			it->ByeByeItem();
			break;

		}
	}
}
