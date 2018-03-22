#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleMapGenerator.h"

PlayerEntity::PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start()
{
	anim = &idleDown;
	state = states::PL_IDLE;

	endPos.x = 250.0f;
	endPos.y = 250.0f;

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
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.x;
				anim = &dashRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleLeft || animBeforeDash == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.x;
				
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
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;

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
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
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
			if (animBeforeDash == &idleRight || animBeforeDash == &right)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.x;
				anim = &dashRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleLeft || animBeforeDash == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.x;

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
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;

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
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * endPos.y;
				anim = &dashDown;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBeforeDash == &idleUpRight || animBeforeDash == &upRight)
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
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
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
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
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
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
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
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN)
		{
			startPos = pos;
			state = states::PL_DASH;
			animBeforeDash = anim;
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

void PlayerEntity::Walk(bool can)
{
	this->move = can;
}

void PlayerEntity::InitCulling()
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

void PlayerEntity::CheckCulling()
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