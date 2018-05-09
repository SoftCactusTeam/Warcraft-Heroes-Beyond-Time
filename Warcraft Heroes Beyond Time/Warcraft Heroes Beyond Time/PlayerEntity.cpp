#include "Application.h"
#include "PlayerEntity.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Scene.h"
#include "ModuleMapGenerator.h"
#include "ModuleEntitySystem.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleItems.h"
#include "GUIWindow.h"
#include "GUIImage.h"

PlayerEntity::PlayerEntity(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity(coor, texture, DynamicType::PLAYER), type(type)
{
	App->entities->LoadCDs(DashConfigCD, damagedConfigCD, deadinfloorConfigCD);
}

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

bool PlayerEntity::Finish() 
{ 
	return true; 
}

Collider* PlayerEntity::GetDamageCollider() const
{
	return damageCol;
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
	firstArgument.y = subT3 * startPos.y;

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

void PlayerEntity::ResetDash()
{
	state = states::PL_IDLE;
	t = 0.0f;
	DashCD = DashConfigCD;

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
		{
			if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
			{
				state = states::PL_UP_RIGHT;
				anim = &upRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				state = states::PL_UP_LEFT;
				anim = &upLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				state = states::PL_DOWN_RIGHT;
				anim = &downRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				state = states::PL_DOWN_LEFT;
				anim = &downLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				state = states::PL_UP;
				anim = &up;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				state = states::PL_DOWN;
				anim = &down;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				state = states::PL_LEFT;
				anim = &left;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				state = states::PL_RIGHT;
				anim = &right;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && t == 0.0f && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				state = states::PL_DASH;
				startPos = pos;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_DASH:
		{
			if (t <= 1.0f && t >= 0.0f)
			{
				if (animBefore == &idleRight || animBefore == &right)
				{
					pos.x = startPos.x + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

					anim = &dashRight;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleLeft || animBefore == &left)
				{
					pos.x = startPos.x - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

					anim = &dashLeft;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleUp || animBefore == &up)
				{
					pos.y = startPos.y - CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

					anim = &dashUp;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleDown || animBefore == &down)
				{
					pos.y = startPos.y + CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f }).y * dashDistance;

					anim = &dashDown;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleUpRight || animBefore == &upRight)
				{
					fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

					pos.x = startPos.x + dashDistance * 0.75f * bezierPoint.y;
					pos.y = startPos.y - dashDistance * 0.75f * bezierPoint.y;

					anim = &dashUpRight;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleDownRight || animBefore == &downRight)
				{
					fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

					pos.x = startPos.x + dashDistance * 0.75f * bezierPoint.y;
					pos.y = startPos.y + dashDistance * 0.75f * bezierPoint.y;

					anim = &dashDownRight;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleDownLeft || animBefore == &downLeft)
				{
					fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

					pos.x = startPos.x - dashDistance * 0.75f * bezierPoint.y;
					pos.y = startPos.y + dashDistance * 0.75f * bezierPoint.y;

					anim = &dashDownLeft;
					float x = 0.05f / dt;
					t += (x * dt);
				}
				else if (animBefore == &idleUpLeft || animBefore == &upLeft)
				{
					fPoint bezierPoint = CalculatePosFromBezier({ 0.0f, 0.0f }, handleA, t, handleB, { 1.0f, 1.0f });

					pos.x = startPos.x - dashDistance * 0.75f * bezierPoint.y;
					pos.y = startPos.y - dashDistance * 0.75f * bezierPoint.y;

					anim = &dashUpLeft;
					float x = 0.05f / dt;
					t += (x * dt);
				}
			}
			else
			{
				ResetDash();
			}
			break;
		}

		case states::PL_UP:
		{
			pos.y -= numStats.speed * dt;

			if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_UP))
			{
				state = states::PL_IDLE;
				anim = &idleUp;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				state = states::PL_UP_RIGHT;
				anim = &upRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				state = states::PL_UP_LEFT;
				anim = &upLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_DOWN:
		{
			pos.y += numStats.speed * dt;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleDown;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				state = states::PL_DOWN_RIGHT;
				anim = &downRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				state = states::PL_DOWN_LEFT;
				anim = &downLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_LEFT:
		{
			pos.x -= numStats.speed * dt;
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				state = states::PL_UP_LEFT;
				anim = &upLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				state = states::PL_DOWN_LEFT;
				anim = &downLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_RIGHT:
		{
			pos.x += numStats.speed * dt;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				state = states::PL_UP_RIGHT;
				anim = &upRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				state = states::PL_DOWN_RIGHT;
				anim = &downRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_UP_LEFT:
		{
			pos.x -= numStats.speed * 0.75f * dt;
			pos.y -= numStats.speed * 0.75f * dt;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleUpLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
			{
				state = states::PL_LEFT;
				anim = &left;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				state = states::PL_UP;
				anim = &up;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_UP_RIGHT:
		{
			pos.x += numStats.speed * 0.75f * dt;
			pos.y -= numStats.speed * 0.75f * dt;
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleUpRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
			{
				state = states::PL_RIGHT;
				anim = &right;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				state = states::PL_UP;
				anim = &up;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_DOWN_LEFT:
		{
			pos.x -= numStats.speed * 0.75f * dt;
			pos.y += numStats.speed * 0.75f * dt;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleDownLeft;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
			{
				state = states::PL_LEFT;
				anim = &left;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			{
				state = states::PL_DOWN;
				anim = &down;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_DOWN_RIGHT:
		{
			pos.x += numStats.speed * 0.75f * dt;
			pos.y += numStats.speed * 0.75f * dt;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP && App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				state = states::PL_IDLE;
				anim = &idleDownRight;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
			{
				state = states::PL_RIGHT;
				anim = &right;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				state = states::PL_DOWN;
				anim = &down;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && DashCD == 0.0f)
			{
				App->audio->PlayFx(App->audio->Thrall_Dash_FX);
				startPos = pos;
				state = states::PL_DASH;
				animBefore = anim;
			}
			else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
			{
				state = states::PL_ATTACK;
				animBefore = anim;
				Attack();
			}
			else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && numStats.energy == 100)
			{
				state = states::PL_SKILL;
				animBefore = anim;
				anim = &skill;
				numStats.energy = 0;
				UseSkill();
			}
			break;
		}

		case states::PL_ATTACK:
		{
			if (animBefore == &idleDown || animBefore == &down)
				anim = &attackDown;

			else if (animBefore == &idleUp || animBefore == &up)
				anim = &attackUp;

			else if (animBefore == &idleUpRight || animBefore == &upRight)
				anim = &attackUpRight;

			else if (animBefore == &idleRight || animBefore == &right)
				anim = &attackRight;

			else if (animBefore == &idleDownRight || animBefore == &downRight)
				anim = &attackDownRight;

			else if (animBefore == &idleDownLeft || animBefore == &downLeft)
				anim = &attackDownLeft;

			else if (animBefore == &idleLeft || animBefore == &left)
				anim = &attackLeft;

			else if (animBefore == &idleUpLeft || animBefore == &upLeft)
				anim = &attackUpLeft;

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
			}

			break;
		}

		case states::PL_SKILL:
		{
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
			}
			break;
		}

		case states::PL_DEAD:
		{
			if (anim->Finished() && anim != &deadDownRight)
			{
				anim->Reset();
				animBefore = anim;
				anim = &deadDownRight;
			}
			else if (anim->Finished())
			{
				deadinfloorcd += dt;
				// PlayFX, Go to the main menu.
				if (deadinfloorcd > deadinfloorConfigCD)
				{
					anim->Reset();
					deadinfloorcd = 0.0f;
					App->scene->GoMainMenu();
				}
			}
			break;
		}

		case states::PL_WIN:
		{
			afterWinCounter += dt;
			if (afterWinCounter > 20)
			{
				App->scene->GoMainMenu();
			}
			else if (afterWinCounter > 2)
			{
				App->scene->CreateGratitudeScreen();
			}
			break;
		}
	}

	if (DashCD > 0.0f)
	{
		DashCD -= dt;
		if (DashCD < 0.0f)
			DashCD = 0.0f;
	}

	if (damaged)
	{
		damagedCD += dt;
		
		uint percent = damagedCD * 100 / damagedConfigCD;
		float alpha = 255 - (percent * 255 / 100);
		App->scene->blood->setOpacity(alpha);

		if (damagedCD > damagedConfigCD)
		{
			SDL_SetTextureColorMod(App->entities->spritesheetsEntities[THRALL_SHEET], 255, 255, 255);
			damaged = false;
			damagedCD = 0.0f;
			
			App->gui->DestroyElem(App->scene->blood);
			App->scene->blood = nullptr;
		}
	}
}

void PlayerEntity::JoyconStates(float dt)
{
	switch (state)
	{
	case states::PL_IDLE:
	{
		if (App->input->GetXAxis() != 0 || App->input->GetYAxis() != 0)
		{
			state = states::PL_MOVE;
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f && DashCD == 0.0f)
		{
			App->audio->PlayFx(App->audio->Thrall_Dash_FX);
			App->input->PlayJoyRumble(0.85f, 100);
			startPos = pos;
			state = states::PL_DASH;
			animBefore = anim;
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
		{
			animBefore = anim;
			state = states::PL_ATTACK;
			Attack();
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN && numStats.energy == 100)
		{
			animBefore = anim;
			anim = &skill;
			numStats.energy = 0;
			state = states::PL_SKILL;
			UseSkill();
		}
		break;
	}


	case states::PL_DASH:
	{
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
			DashCD = DashConfigCD;
			animBefore = nullptr;
			t = 0.0f;
		}

		break;
	}

	case states::PL_MOVE:
	{
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KEY_DOWN && t == 0.0f && DashCD == 0.0f)
		{
			App->audio->PlayFx(App->audio->Thrall_Dash_FX);
			App->input->PlayJoyRumble(0.75f, 100);
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

		pos.x += X * numStats.speed * dt;
		pos.y += Y * numStats.speed * dt;

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
			Attack();
			break;
		}

		else if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN && numStats.energy == 100)
		{
			animBefore = anim;
			anim = &skill;
			numStats.energy = 0;
			state = states::PL_SKILL;
			UseSkill();
		}
		break;
	}

	case states::PL_ATTACK:
	{
		if (animBefore == &idleDown || animBefore == &down)
			anim = &attackDown;

		else if (animBefore == &idleUp || animBefore == &up)
			anim = &attackUp;

		else if (animBefore == &idleLeft || animBefore == &left)
			anim = &attackLeft;

		else if (animBefore == &idleRight || animBefore == &right)
			anim = &attackRight;

		else if (animBefore == &upLeft || animBefore == &idleUpLeft)
			anim = &attackUpLeft;

		else if (animBefore == &upRight || animBefore == &idleUpRight)
			anim = &attackUpRight;

		else if (animBefore == &downLeft || animBefore == &idleDownLeft)
			anim = &attackDownLeft;

		else if (animBefore == &downRight || animBefore == &idleDownRight)
			anim = &attackDownRight;

		if (anim->Finished())
		{
			anim->Reset();
			anim = animBefore;

			if (animBefore == &left || animBefore == &up || animBefore == &right || animBefore == &down || animBefore == &upRight || animBefore == &upLeft || animBefore == &downLeft || animBefore == &downRight)
				state = states::PL_MOVE;
			else
				state = states::PL_IDLE;
		}
		break;
	}

	case states::PL_SKILL:
	{
		if (anim->Finished())
		{

			anim->Reset();
			anim = animBefore;

			if (animBefore == &left || animBefore == &up || animBefore == &right || animBefore == &down || animBefore == &upRight || animBefore == &upLeft || animBefore == &downLeft || animBefore == &downRight)
				state = states::PL_MOVE;
			else
				state = states::PL_IDLE;
		}
		break;
	}

	case states::PL_DEAD:
	{
		if (anim != &deadDownRight)
		{
			anim->Reset();
			animBefore = anim;
			anim = &deadDownRight;
		}
		else if (anim->Finished())
		{
			deadinfloorcd += dt;
			// PlayFX, Go to the main menu.
			if (deadinfloorcd > deadinfloorConfigCD)
			{
				anim->Reset();
				deadinfloorcd = 0.0f;
				App->scene->GoMainMenu();
			}
		}
		break;
	}

	case states::PL_WIN:
	{
		afterWinCounter += dt;
		if (afterWinCounter > 20)
		{
			App->scene->GoMainMenu();
		}
		else if (afterWinCounter > 2 && !App->scene->gratitudeON)
		{
			App->scene->CreateGratitudeScreen();
		}
		break;
	}
	}

	if (DashCD > 0.0f)
	{
		DashCD -= dt;
		if (DashCD < 0.0f)
			DashCD = 0.0f;
	}

	if (damaged)
	{
		damagedCD += dt;

		uint percent = damagedCD * 100 / damagedConfigCD;
		Uint8 alpha = 255 - (percent * 255 / 100);
		App->scene->blood->setOpacity(alpha);

		if (damagedCD > damagedConfigCD)
		{
			SDL_SetTextureColorMod(App->entities->spritesheetsEntities[THRALL_SHEET], 255, 255, 255);
			damaged = false;
			damagedCD = 0.0f;
		
			App->gui->DestroyElem(App->scene->blood);
			App->scene->blood = nullptr;
		}

		
	}
}

bool PlayerEntity::getConcretePlayerStates(int stat)
{
	if (stat == (int)state)
		return true;
	return false;
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
	case states::PL_DASH:
		anim = &idleDown;
		state = states::PL_IDLE;
		break;
	case states::PL_ATTACK:
		anim = &idleDown;
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

FIXED_ANGLE PlayerEntity::returnFixedAngle()
{
	if (angle >= 247.5f && angle < 292.5f)
		return UP;
	else if (angle >= 67.5f && angle < 112.5f)
		return DOWN;
	else if ((angle >= 337.5f && angle < 360.0f) || (angle >= 0 && angle < 22.5f))
		return RIGHT;
	else if (angle >= 157.5f && angle < 202.5f)
		return LEFT;
	else if (angle >= 292.5f && angle < 337.5f)
		return UP_RIGHT;
	else if (angle >= 202.5f && angle < 247.5f)
		return UP_LEFT;
	else if (angle >= 112.5f && angle < 157.5f)
		return DOWN_LEFT;
	else if (angle >= 22.5f && angle < 67.5f)
		return DOWN_RIGHT;
	else
		return UP;
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
		iPoint pivot = anim->GetCurrentPivot();

		App->render->fcamerax = -1 * (this->pos.x - pivot.x + currRect.w / 2 - App->render->camera.w / 2);
		App->render->fcameray = -1 * (this->pos.y - pivot.y + currRect.h / 2 - App->render->camera.h / 2);

		freeZonex = pos.x - pivot.x - 55 / 2;
		freeZoney = pos.y - pivot.y - 55 / 2;

		freeZone.x = pos.x - pivot.x - 55;
		freeZone.y = pos.y - pivot.y - 55;
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
		tilesize = App->map->getTileSize() + 2;
		SDL_Rect currentRect = anim->GetCurrentRect();
		currentRect.w = wallCol->rectArea.w;
		currentRect.h = wallCol->rectArea.h;
		//iPoint pivot = anim->GetCurrentPivot();
		iPoint pivot = { wallCol->rectArea.x, wallCol->rectArea.y };//false
		fPoint topleft = { pos.x + pivot.x, pos.y + pivot.y };

		if (freeZonex > topleft.x && -App->render->camera.x > 0)
		{
			if (App->render->fcamerax + freeZonex - topleft.x > 0)
			{
				freeZonex -= -App->render->fcamerax;
				App->render->fcamerax = 0;
			}
			else
			{
				App->render->fcamerax += freeZonex - topleft.x;
				freeZonex = topleft.x;
			}
		}


		else if (freeZonex + freeZone.w < topleft.x + currentRect.w  && -App->render->camera.x < w * (tilesize - 2) - App->render->camera.w)
		{
			App->render->fcamerax -= (topleft.x + currentRect.w) - (freeZonex + freeZone.w);
			freeZonex = (topleft.x + currentRect.w) - freeZone.w;
		}

		if (freeZoney > topleft.y && -App->render->camera.y > 0)
		{
			if (App->render->fcameray + freeZoney - topleft.y > 0)
			{
				freeZoney -= -App->render->fcameray;
				App->render->fcameray = 0;
			}
			else
			{
				App->render->fcameray += freeZoney - topleft.y;
				freeZoney = topleft.y;
			}

		}
		else if (freeZoney + freeZone.h < topleft.y + currentRect.h && -App->render->camera.y + App->render->camera.h < h * (tilesize - 2))
		{
			App->render->fcameray -= (topleft.y + currentRect.h) - (freeZoney + freeZone.h);
			freeZoney = topleft.y + currentRect.h - freeZone.h;
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

		else if (pos.x + 55 - this->anim->GetCurrentPivot().x > w * (48 - 2))
		{
			pos.x = w * (48 - 2) - 55 + this->anim->GetCurrentPivot().x;
		}

		if (pos.y - this->anim->GetCurrentPivot().y < 0)
			pos.y = 0 + this->anim->GetCurrentPivot().y;

		else if (pos.y + 47 - this->anim->GetCurrentPivot().y > h * (48 - 2))
		{
			pos.y = h * (48 - 2) - 47 + this->anim->GetCurrentPivot().y;
		}
	}
}

void PlayerEntity::SetDamage(int damage, bool setStateDamage)
{
	if (numStats.hp > 0 && damaged == false)
	{
		if ((int)numStats.hp - damage <= 0)
		{
			numStats.hp = 0;
			ResetDash();
			state = states::PL_DEAD;
			App->audio->PauseMusic(0.5);
			App->audio->PlayFx(App->audio->Thrall_Die_FX);
		}
		else
		{
			GUIWindow* blood = (GUIWindow*)App->gui->CreateGUIWindow({ 0,0 }, { 0,0,0,0 }, nullptr, nullptr);
			blood->menu = false;
			GUIImage* image = (GUIImage*)App->gui->CreateGUIImage({ 0,0 }, { 0, 912, 640, 360 }, nullptr, blood);
			App->scene->blood = blood;
			
			App->audio->PlayFx(App->audio->Thrall_Hitted_FX);
			damaged = true;
			SDL_SetTextureColorMod(App->entities->spritesheetsEntities[THRALL_SHEET], 255, 100, 100);
			numStats.hp -= damage;
		}
	}
}

void PlayerEntity::DrawFreeZone(bool boolean)
{
	drawFZ = boolean;
}

void PlayerEntity::PushOut(Collider* wall)
{
	bool collideByRight = false, collideByLeft = false, collideByTop = false, collideByBottom = false;
	SDL_Rect wall_r = wall->rectArea;
	SDL_Rect player_col = { wallCol->rectArea.x + (int)pos.x, wallCol->rectArea.y + (int)pos.y, wallCol->rectArea.w, wallCol->rectArea.h };

	if (wall->rectArea.x + wall->rectArea.w / 2 <= wallCol->rectArea.x + (int)pos.x)
		collideByRight = true;

	else if (wall->rectArea.x + wall->rectArea.w / 2 > wallCol->rectArea.x + (int)pos.x + wallCol->rectArea.w)
		collideByLeft = true;

	if (wall->rectArea.y + wall->rectArea.h / 2 < wallCol->rectArea.y + (int)pos.y)
		collideByBottom = true;

	else if (wall->rectArea.y + wall->rectArea.h / 2 >= wallCol->rectArea.y + (int)pos.y + wallCol->rectArea.h)
		collideByTop = true;

	//4 main direction collisions
	if (collideByRight && !collideByBottom && !collideByTop)
	{
		pos.x += (wall->rectArea.x + wall->rectArea.w - (wallCol->rectArea.x + (int)pos.x));
	}
	else if (collideByLeft && !collideByTop && !collideByBottom)
	{
		pos.x -= (wallCol->rectArea.x + wallCol->rectArea.w + pos.x) - wall->rectArea.x;
	}
	else if (collideByTop && !collideByLeft && !collideByRight)
	{
		pos.y -= (wallCol->rectArea.y + (int)pos.y + wallCol->rectArea.h) - wall->rectArea.y;
	}
	else if (collideByBottom && !collideByLeft && !collideByRight)
	{
		pos.y += wall->rectArea.y + wall->rectArea.h - (wallCol->rectArea.y + (int)pos.y);
	}

	//Combination between them (choose the closest direction)
	else if (collideByTop && collideByRight)
	{
		if ((player_col.y + player_col.h) - wall_r.y < (wall_r.x + wall_r.w - player_col.x))
		{
			pos.y -= (wallCol->rectArea.y + (int)pos.y + wallCol->rectArea.h) - wall->rectArea.y;
		}
		else
		{
			pos.x += (wall->rectArea.x + wall->rectArea.w) - ((int)pos.x + wallCol->rectArea.x);
		}
	}
	else if (collideByTop && collideByLeft)
	{
		if ((player_col.y + player_col.h) - wall_r.y < (player_col.x + player_col.w - wall_r.x))
		{
			pos.y -= (player_col.y + player_col.h) - wall_r.y;
		}
		else
		{
			pos.x -= player_col.x + player_col.w - wall_r.x;
		}
	}
	else if (collideByBottom && collideByRight)
	{
		if ((wall_r.y + wall_r.h - player_col.y) < (wall_r.x + wall_r.w - player_col.x))
		{
			pos.y += (wall_r.y + wall_r.h - player_col.y);
		}
		else
		{
			pos.x += (wall_r.x + wall_r.w - player_col.x);
		}
	}
	else if (collideByBottom && collideByLeft)
	{
		if ((wall_r.y + wall_r.h - player_col.y) < (player_col.x + player_col.w - wall_r.x))
		{
			pos.y += (wall_r.y + wall_r.h - player_col.y);
		}
		else
		{
			pos.x -= (player_col.x + player_col.w - wall_r.x);
		}
	}
}

void PlayerEntity::IncreaseEnergy(int percent)
{
	if (numStats.energy + percent < 100)
		numStats.energy += percent;

	else if (numStats.energy < 100)
	{
		numStats.energy = 100;
		App->audio->PlayFx(App->audio->Thrall_EnergyMax_FX);
	}

}

bool PlayerEntity::Draw()
{
	bool ret = true;

	if (damaged)
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot(), ModulePrinter::Pivots::UPPER_LEFT, { 0,0 }, 0, { 255,100,100,255 });
	else
		ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame(), 0, ModulePrinter::Pivots::CUSTOM_PIVOT, anim->GetCurrentPivot());

	return ret;
}