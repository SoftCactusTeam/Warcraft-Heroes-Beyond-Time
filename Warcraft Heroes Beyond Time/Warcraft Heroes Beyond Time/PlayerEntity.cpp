#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Scene.h"
#include "ModuleMapGenerator.h"
#include "ModuleEntitySystem.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

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

void PlayerEntity::Collision(COLLIDER_TYPE type){}

void PlayerEntity::setCol(Collider* pcol)
{
	this->pcol = pcol;
}

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
		if (drawFZ)
			App->printer->PrintQuad(freeZone, { 255, 0, 0, 50 }, true, true);
	}
	else
	{
		CheckIddleStates();
		if (drawFZ)
			App->printer->PrintQuad(freeZone, { 255, 0, 0, 50 }, true, true);

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
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
			break;
		}

		break;

	case states::PL_DASH:

		if (t <= 1.0f && t >= 0.0f)
		{
			if (animBefore == &idleRight || animBefore == &right)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleLeft || animBefore == &left)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashLeft;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleUp || animBefore == &up)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashUp;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleDown || animBefore == &down)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

				anim = &dashDown;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleUpRight || animBefore == &upRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * 0.75f * bezierPoint.y;
				pos.y = startPos.y - dashDistance * 0.75f * bezierPoint.y;

				anim = &dashUpRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleDownRight || animBefore == &downRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * 0.75f * bezierPoint.y;
				pos.y = startPos.y + dashDistance * 0.75f * bezierPoint.y;

				anim = &dashDownRight;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleDownLeft || animBefore == &downLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x - dashDistance * 0.75f * bezierPoint.y;
				pos.y = startPos.y + dashDistance * 0.75f * bezierPoint.y;

				anim = &dashDownLeft;
				float x = 0.05f / dt;
				t += (x * dt);
				break;
			}
			else if (animBefore == &idleUpLeft || animBefore == &upLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x - dashDistance * 0.75f * bezierPoint.y;
				pos.y = startPos.y - dashDistance * 0.75f * bezierPoint.y;

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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
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
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			state = states::PL_ATTACK;
			animBefore = anim;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			state = states::PL_SKILL;
			animBefore = anim;
			anim = &skill;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
			break;
		}

		break;

	case states::PL_ATTACK:

		if (animBefore == &idleDown || animBefore == &down)
			anim = &attackDown;

		else if (animBefore == &idleUp || animBefore == &up)
			anim = &attackUp;

		else if (animBefore == &idleLeft || animBefore == &left || animBefore == &upLeft || animBefore == &downLeft || animBefore == &idleDownLeft || animBefore == &idleUpLeft)
			anim = &attackLeft;

		else if (animBefore == &idleRight || animBefore == &right || animBefore == &idleUpRight || animBefore == &idleDownRight || animBefore == &upRight || animBefore == &downRight)
			anim = &attackRight;

		if (anim->Finished())
		{
			anim->Reset();

			if (animBefore == &left)
				anim = &idleLeft;
			else if (animBefore == &up)
				anim = &idleUp;
			else if (animBefore == &down)
				anim = &idleDown;
			else if (animBefore == &right)
				anim = &idleRight;
			else if (animBefore == &upRight)
				anim = &idleUpRight;
			else if (animBefore == &upLeft)
				anim = &idleUpLeft;
			else if (animBefore == &downLeft)
				anim = &idleDownLeft;
			else if (animBefore == &downRight)
				anim = &idleDownRight;
			else
				anim = animBefore;

			state = states::PL_IDLE;

			break;
		}

		break;

	case states::PL_SKILL:

		if (anim->Finished())
		{
			anim->Reset();
			anim = animBefore;

			if (animBefore == &left)
				anim = &idleLeft;
			else if (animBefore == &up)
				anim = &idleUp;
			else if (animBefore == &down)
				anim = &idleDown;
			else if (animBefore == &right)
				anim = &idleRight;
			else if (animBefore == &upRight)
				anim = &idleUpRight;
			else if (animBefore == &upLeft)
				anim = &idleUpLeft;
			else if (animBefore == &downLeft)
				anim = &idleDownLeft;
			else if (animBefore == &downRight)
				anim = &idleDownRight;
			else
				anim = animBefore;

			state = states::PL_IDLE;

			break;
		}

		break;

	case states::PL_DEAD:

		if (anim->Finished())
		{
			anim->Reset();
			// DO SOMETHING!!
			damaged = false;
			break;
		}

		break;

	case states::PL_DAMAGE:

		if (anim->Finished())
		{
			anim->Reset();
			anim = &idleDown;
			damaged = false;
			state = states::PL_IDLE;
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
			animBefore = anim;
			break;
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			animBefore = anim;
			state = states::PL_ATTACK;
			break;
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
		{
			animBefore = anim;
			anim = &skill;
			state = states::PL_SKILL;
			break;
		}
		else if (live <= 0)
		{
			state = states::PL_DEAD;
			animBefore = anim;
			anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
			break;
		}
		else if (damaged)
		{
			state = states::PL_DAMAGE;
			animBefore = anim;
			anim = &damagedAnim;
			break;
		}

		break;

	case states::PL_DASH:

		if (t <= 1.0f && t >= 0.0f)
		{
			if (animBefore == &idleRight)
			{
				pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashRight;
				
			}
			else if (animBefore == &idleLeft)
			{
				pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashLeft; //
			}
			else if (animBefore == &idleUp)
			{
				pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashUp; //
			}
			else if (animBefore == &idleDown)
			{
				pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;
				anim = &dashDown;
			}
			else if (animBefore == &idleUpRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(315.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(315.0f));

				anim = &dashUpRight;
			}
			else if (animBefore == &idleUpLeft)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(225.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(225.0f));

				anim = &dashUpLeft; //
			}
			else if (animBefore == &idleDownRight)
			{
				fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

				pos.x = startPos.x + dashDistance * bezierPoint.y * cos(DEG_2_RAD(45.0f));
				pos.y = startPos.y + dashDistance * bezierPoint.y * sin(DEG_2_RAD(45.0f));

				anim = &dashDownRight;
			}
			else if (animBefore == &idleDownLeft)
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
				anim = animBefore;
			}
				
			animBefore = nullptr;
			t = 0.0f;
		}

		break;

		case states::PL_MOVE:
		{
			if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f)
			{
				animBefore = anim;
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

			if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
			{
				animBefore = anim;
				state = states::PL_ATTACK;

				break;

			}

			else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
			{
				animBefore = anim;
				anim = &skill;
				state = states::PL_SKILL;
				break;
			}
			else if (live <= 0)
			{
				state = states::PL_DEAD;
				animBefore = anim;
				anim = &damagedAnim; // SWAP TO DEAD ANIM WHEN ITS DONE
				break;
			}
			else if (damaged)
			{
				state = states::PL_DAMAGE;
				animBefore = anim;
				anim = &damagedAnim;
				break;
			}

			break;
		}

		case states::PL_ATTACK:
			if (animBefore == &idleDown || animBefore == &down)
				anim = &attackDown;

			else if (animBefore == &idleUp || animBefore == &up)
				anim = &attackUp;

			else if (animBefore == &idleLeft || animBefore == &left || animBefore == &upLeft || animBefore == &downLeft || animBefore == &idleDownLeft || animBefore == &idleUpLeft)
				anim = &attackLeft;

			else if (animBefore == &idleRight || animBefore == &right || animBefore == &idleUpRight || animBefore == &idleDownRight || animBefore == &upRight || animBefore == &downRight)
				anim = &attackRight;

			if (anim->Finished())
			{
				anim->Reset();
				anim = animBefore;

				if (animBefore == &left || animBefore == &up || animBefore == &right || animBefore == &down || animBefore == &upRight || animBefore == &upLeft || animBefore == &downLeft || animBefore == &downRight)
					state = states::PL_MOVE;
				else
					state = states::PL_IDLE;

				break;
			}

		break;

		case states::PL_SKILL:

			if (anim->Finished())
			{

				anim->Reset();
				anim = animBefore;

				if (animBefore == &left || animBefore == &up || animBefore == &right || animBefore == &down || animBefore == &upRight || animBefore == &upLeft || animBefore == &downLeft || animBefore == &downRight)
					state = states::PL_MOVE;
				else
					state = states::PL_IDLE;

				break;
			}

		break;

		case states::PL_DEAD:

			if (anim->Finished())
			{
				anim->Reset();
				// DO SOMETHING
				damaged = false;
				break;
			}

			break;

		case states::PL_DAMAGE:

			if (anim->Finished())
			{
				anim->Reset();
				anim = &idleDown;
				damaged = false;
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
	if (this == App->scene->player)
	{
		SDL_Rect currRect = anim->GetCurrentRect();
		App->render->fcamerax = -1 * (this->pos.x + currRect.w / 2 - App->render->camera.w / 2);
		App->render->fcameray = -1 * (this->pos.y + currRect.h / 2 - App->render->camera.h / 2);

		freeZonex = pos.x - 55/2;
		freeZoney = pos.y - 55/2;

		freeZone.x = pos.x - 55;
		freeZone.y = pos.y - 55;
		freeZone.w = 55 / 2 * 2 + 55;
		freeZone.h = 55 / 2 * 2 + 47;
	}
}

void PlayerEntity::CheckCulling()
{
	if (this == App->scene->player)
	{
		uint w, h;
		int tilesize;
		App->map->getSize(w, h);
		tilesize = App->map->getTileSize();
		SDL_Rect currentRect = anim->GetCurrentRect();

		if (freeZonex > this->pos.x - this->anim->GetCurrentPivot().x && -App->render->camera.x > 0)
		{
			if (App->render->fcamerax + freeZonex - pos.x - this->anim->GetCurrentPivot().x > 0)
			{
				freeZonex -= -App->render->fcamerax;
				App->render->fcamerax = 0;
			}
			else
			{
				App->render->fcamerax += freeZonex - pos.x - this->anim->GetCurrentPivot().x;
				freeZonex = this->pos.x;
			}
		}
		

		else if (freeZonex + freeZone.w < pos.x + currentRect.w - this->anim->GetCurrentPivot().x && - App->render->camera.x < w * (tilesize-2) - App->render->camera.w)
		{
			App->render->fcamerax -= (pos.x + currentRect.w - this->anim->GetCurrentPivot().x) - (freeZonex + freeZone.w);
			freeZonex = (this->pos.x + currentRect.w - this->anim->GetCurrentPivot().x) - freeZone.w;
		}

		if (freeZoney > pos.y - this->anim->GetCurrentPivot().y && -App->render->camera.y > 0)
		{
			if (App->render->fcameray + freeZoney - pos.y - this->anim->GetCurrentPivot().y > 0)
			{
				freeZoney -= -App->render->fcameray;
				App->render->fcameray = 0;
			}
			else
			{
				App->render->fcameray += freeZoney - pos.y - this->anim->GetCurrentPivot().y;
				freeZoney = pos.y - this->anim->GetCurrentPivot().y;
			}
			
		}
		else if (freeZoney + freeZone.h < pos.y + currentRect.h - this->anim->GetCurrentPivot().y && -App->render->camera.y + App->render->camera.h < h * (tilesize-2))
		{
			App->render->fcameray -= (pos.y + currentRect.h - this->anim->GetCurrentPivot().y) - (freeZoney + freeZone.h);
			freeZoney = pos.y + currentRect.h - this->anim->GetCurrentPivot().y - freeZone.h;
		}

		freeZone.x = (int)freeZonex;
		freeZone.y = (int)freeZoney;

		
	}
}

void PlayerEntity::CheckMapLimits()
{
	uint w, h;
	App->map->getSize(w, h);

	if (w != 0 && h != 0)
	{
		if (pos.x - this->anim->GetCurrentPivot().x < 0)
			pos.x = 0 + this->anim->GetCurrentPivot().x;

		else if (pos.x + 55 - this->anim->GetCurrentPivot().x > w * (48-2))
		{
			pos.x = w * (48-2) - 55 + this->anim->GetCurrentPivot().x;
		}

		if (pos.y - this->anim->GetCurrentPivot().y < 0)
			pos.y = 0 + this->anim->GetCurrentPivot().y;

		else if (pos.y + 47 - this->anim->GetCurrentPivot().y > h * (48-2))
		{
			pos.y = h * (48-2) - 47 + this->anim->GetCurrentPivot().y;
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

void PlayerEntity::SetDamage(int damage, bool setStateDamage)
{
	if (setStateDamage)
		damaged = true;
	live -= damage;
}

void PlayerEntity::DrawFreeZone(bool boolean)
{
	drawFZ = boolean;
}