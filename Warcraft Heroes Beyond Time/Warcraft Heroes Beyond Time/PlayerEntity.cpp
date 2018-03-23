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
		dashDistance = pos.x + 30.0f;
		dashDistance = pos.y;
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
				
				if (t >= 0.0f && t <= 0.2f)
					anim = &animDashLeft[1];
				else if (t > 0.2f && t <= 0.4f)
					anim = &animDashLeft[2];
				else if (t > 0.4f && t <= 0.55f)
					anim = &animDashLeft[3];
				else if (t > 0.55f && t <= 0.7f)
					anim = &animDashLeft[4];
				else if (t > 0.7f && t <= 1.0f)
					anim = &animDashLeft[5];


				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleUp || animBeforeDash == &up)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				if (t >= 0.0f && t <= 0.2f)
					anim = &animDashUp[1];
				else if (t > 0.2f && t <= 0.4f)
					anim = &animDashUp[2];
				else if (t > 0.4f && t <= 0.55f)
					anim = &animDashUp[3];
				else if (t > 0.55f && t <= 0.7f)
					anim = &animDashUp[4];
				else if (t > 0.7f && t <= 1.0f)
					anim = &animDashUp[5];

				/*if (t >= 0.0f && t <= 0.1835f)
				{
					anim = &animDashUp[1];
				}
				else if (t > 0.1835f && t <= 0.3395f)
				{
					anim = &animDashUp[2];
				}
				else if (t > 0.3395f && t <= 0.4955f)
				{
					anim = &animDashUp[3];
				}
				else if (t > 0.4955f && t <= 0.5595f)
				{
					anim = &animDashUp[4];
				}
				else if (t > 0.5595f && t <= 0.5735f)
				{
					anim = &animDashUp[5];
				}
				else if (t > 0.5735f && t <= 0.6375f)
				{
					anim = &animDashUp[6];
				}
				else if (t > 0.6375f && t <= 0.7935f)
				{
					anim = &animDashUp[7];
				}
				else if (t > 0.7935f && t <= 1.0f)
				{
					anim = &animDashUp[8];
				}*/

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

				if (t >= 0.0f && t <= 0.2f)
					anim = &animDashUpLeft[1];
				else if (t > 0.2f && t <= 0.4f)
					anim = &animDashUpLeft[2];
				else if (t > 0.4f && t <= 0.55f)
					anim = &animDashUpLeft[3];
				else if (t > 0.55f && t <= 0.7f)
					anim = &animDashUpLeft[4];
				else if (t > 0.7f && t <= 1.0f)
					anim = &animDashUpLeft[5];

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
			else if (anim == &animDashLeft[5])
				anim = &idleLeft;
			else if (anim == &animDashUp[5])
				anim = &idleUp;
			else if (anim == &dashDown)
				anim = &idleDown;
			else if (anim == &dashUpRight)
				anim = &idleUpRight;
			else if (anim == &animDashUpLeft[5])
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
				
			}
			else if (animBeforeDash == &idleLeft)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				
			}
			else if (animBeforeDash == &idleUp)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				
			}
			else if (animBeforeDash == &idleDown)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

			}
			else if (animBeforeDash == &idleUpRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(315.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(315.0f));
			}
			else if (animBeforeDash == &idleUpLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(225.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(225.0f));
			}
			else if (animBeforeDash == &idleDownRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(45.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(45.0f));
			}
			else if (animBeforeDash == &idleDownLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(135.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(135.0f));
			}
			else
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance * cos(DEG_2_RAD(angle));
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance * sin(DEG_2_RAD(angle));
			}
			
			float x = 0.05f / dt;
			t += (x * dt);

		}
		else
		{
			state = states::PL_IDLE;
			animBeforeDash = nullptr;
			t = 0.0f;
		}

		break;

		case states::PL_MOVE:
	
			if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f)
			{
				startPos = pos;
				state = states::PL_DASH;

				float X = App->input->GetXAxis() / MAX_JAXIS_VALUE;
				float Y = App->input->GetYAxis() / MAX_JAXIS_VALUE;

				angle = GetAngleFromAxis(X, Y);

				break;
			}

			float X = App->input->GetXAxis() / MAX_JAXIS_VALUE;
			float Y = App->input->GetYAxis() / MAX_JAXIS_VALUE;

			pos.x += X * speed * dt;
			pos.y += Y * speed * dt;

			angle = GetAngleFromAxis(X, Y);

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

float PlayerEntity::GetAngleFromAxis(float X, float Y)
{

	float angle = RAD_2_DEG(atan2(Y, X));

	if (angle < 0)
		angle += 360.0f;

	return angle;
}

Animation* PlayerEntity::GetAnimFromAngle(float angle)
{
	Animation* animToReturn = nullptr;

	if (angle >= 247.5f && angle < 292.5f)
		animToReturn = &up;

	else if (angle >= 67.5f && angle < 112.5f)
		animToReturn = &down;

	else if (((angle >= 337.5f && angle < 360.0f) || (angle >= 0 && angle < 22.5f) && !App->input->InsideDeadZone()))
		animToReturn = &right;

	else if (angle >= 157.5f && angle < 202.5f)
		animToReturn = &left;

	else if (angle >= 292.5f && angle < 337.5f)
		animToReturn = &upRight;

	else if (angle >= 202.5f && angle < 247.5f)
		animToReturn = &upLeft;

	else if (angle >= 112.5f && angle < 157.5f)
		animToReturn = &downLeft;

	else if (angle >= 22.5f && angle < 67.5f)
		animToReturn = &downRight;

	//else
		//animToReturn = &idleDown;

	return animToReturn;
}

void PlayerEntity::Walk(bool can)
{
	this->move = can;
}