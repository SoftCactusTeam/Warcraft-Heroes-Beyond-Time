#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"
#include "ModuleEntitySystem.h"
#include "ModuleColliders.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture)
{
	// Thrall idle animations

	idleUp.PushBack({ 25,15,43,41 },  { 1,0 }); //Example: Introduce here the pivot.
	idleUp.PushBack({ 114,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 203,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 292,15,43,41 }, { 1,0 });
	idleUp.speedFactor = 3.0f;

	idleUpRight.PushBack({ 382,15,34,40 }, { 0,0 });
	idleUpRight.PushBack({ 471,15,34,40 }, { 0,1 });
	idleUpRight.PushBack({ 560,15,34,40 }, { 0,1 });
	idleUpRight.PushBack({ 649,15,34,40 }, { 0,0 });
	idleUpRight.speedFactor = 3.0f;

	idleRight.PushBack({ 34,86,25,44 },  { 0,0 });
	idleRight.PushBack({ 123,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 212,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 301,86,25,44 }, { 0,0 });
	idleRight.speedFactor = 3.0f;

	idleDownRight.PushBack({ 389,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 477,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 566,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 656,86,24,40 }, { 0,0 });
	idleDownRight.speedFactor = 3.0f;

	idleDown.PushBack({ 17,157,48,39 },  { 16,0 });
	idleDown.PushBack({ 106,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 195,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 284,157,48,39 }, { 16,0 });
	idleDown.speedFactor = 3.0f;

	idleDownLeft.PushBack({ 384,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 473,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 562,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 651,162,30,29 }, { 5,0 });
	idleDownLeft.speedFactor = 3.0f;

	idleLeft.PushBack({ 35,225,25,44 },  { 0,4 });
	idleLeft.PushBack({ 124,225,25,44 }, { 0,5 });
	idleLeft.PushBack({ 213,225,25,44 }, { 0,5 });
	idleLeft.PushBack({ 302,225,25,44 }, { 0,4 });
	idleLeft.speedFactor = 3.0f;


	idleUpLeft.PushBack({ 387,233,31,35 }, { 2,0 });
	idleUpLeft.PushBack({ 476,233,31,35 }, { 2,0 });
	idleUpLeft.PushBack({ 565,233,31,35 }, { 2,0 });
	idleUpLeft.PushBack({ 654,233,31,35 }, { 2,0 });
	idleUpLeft.speedFactor = 3.0f;


	// Thrall walk animations
	up.PushBack({ 30,298,43,41 }, { 0,0 });
	up.PushBack({ 118,298,43,41 }, { 0,1 });
	up.PushBack({ 206,298,43,41 }, { 0,1 });
	up.PushBack({ 294,298,43,41 }, { 0,0 });
	up.speedFactor = 9.0f;

	upRight.PushBack({ 380,301,47,41 }, { 5,0 });
	upRight.PushBack({ 468,301,47,41 }, { 5,1 });
	upRight.PushBack({ 558,301,47,41 }, { 5,1 });
	upRight.PushBack({ 647,301,47,41 }, { 5,0 });
	upRight.speedFactor = 9.0f;

	right.PushBack({ 28,372,40,44 },  { 6, 0 });
	right.PushBack({ 118,372,40,44 }, { 6, 1 });
	right.PushBack({ 207,372,40,44 }, { 6, 0 });
	right.PushBack({ 296,372,40,44 }, { 6, 0 });
	right.speedFactor = 9.0f;

	downRight.PushBack({ 383,371,36,40 }, { 8, 1 });
	downRight.PushBack({ 472,371,36,40 }, { 8, 1 });
	downRight.PushBack({ 561,371,36,40 }, { 8, 0 });
	downRight.PushBack({ 650,371,36,40 }, { 8, 1 });
	downRight.speedFactor = 9.0f;

	down.PushBack({ 15,447,48,39 }, {16,0});
	down.PushBack({ 104,447,48,39 }, {16,2});
	down.PushBack({ 193,447,48,39 }, {16,2});
	down.PushBack({ 282,447,48,39 }, {16,2});
	down.speedFactor = 9.0f;

	downLeft.PushBack({ 377,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 466,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 555,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 644,446,41,33 }, { 10, 2 });
	downLeft.speedFactor = 9.0f;

	left.PushBack({ 20,512,40,36 },  { 10,0 });
	left.PushBack({ 109,512,40,36 }, { 10,0 });
	left.PushBack({ 198,512,40,36 }, { 10,0 });
	left.PushBack({ 287,512,40,36 }, { 10,0 });
	left.speedFactor = 9.0f;

	upLeft.PushBack({ 379,513,49,36 }, { 9,0 });
	upLeft.PushBack({ 468,513,49,36 }, { 9,0 });
	upLeft.PushBack({ 557,513,49,36 }, { 9,0 });
	upLeft.PushBack({ 646,513,49,36 }, { 9,0 });
	upLeft.speedFactor = 9.0f;

	//

	// ------- Dash animations -------

	dashUp.PushBack({ 206,298,43,41 }, { 0,1 });
	dashUp.speed = 0.2f;

	dashUpRight.PushBack({ 558,301,47,41 }, { 5,1 });
	dashUpRight.speed = 0.2f;

	dashRight.PushBack({ 207,372,40,44 }, { 6, 0 });
	dashRight.speed = 0.2f;

	dashDownRight.PushBack({ 561,371,36,40 }, { 8, 0 });
	dashDownRight.speed = 0.2f;

	dashDown.PushBack({ 193,447,48,39 }, { 16,2 });
	dashDown.speed = 0.2f;

	dashDownLeft.PushBack({ 555,446,41,33 }, { 10, 2 });
	dashDownRight.speed = 0.2f;

	dashLeft.PushBack({ 198,512,40,36 }, { 10,0 });
	dashLeft.speed = 0.2f;

	dashUpLeft.PushBack({ 557,513,49,36 }, { 9,0 });
	dashUpLeft.speed = 0.2f;

	// ---- attack animation ----

	attackUp.PushBack({ 24,580,46,47 },  { 4,9 });
	attackUp.PushBack({ 114,580,46,47 }, { 4,10});
	attackUp.PushBack({ 202,580,46,47 }, { 4,9 });
	attackUp.PushBack({ 291,580,46,47 }, { 4,8 });
	attackUp.PushBack({ 380,580,46,47 }, { 4,8 });
	attackUp.speedFactor = 9.0f;
	attackUp.loop = false;

	attackRight.PushBack({ 178,639,89,71 });
	attackRight.PushBack({ 267,639,89,71 });
	attackRight.PushBack({ 356,639,89,71 });
	attackRight.PushBack({ 445,639,89,71 });
	attackRight.PushBack({ 534,639,89,71 });
	attackRight.speedFactor = 9.0f;
	attackRight.loop = false;

	attackDown.PushBack({ 356 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 445 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 534 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 623 + 11,710 + 5,89,71 });
	attackDown.PushBack({ 0 + 8,781 + 5,89,71 });
	attackDown.speedFactor = 9.0f;
	attackDown.loop = false;

	attackLeft.PushBack({ 534 - 6,781 + 6,89,71 });
	attackLeft.PushBack({ 623 - 6,781 + 6,89,71 });
	//attackLeft.PushBack({ 0 ,852 + 6,89,71 });
	attackLeft.PushBack({ 89 - 6, 852 + 6,89,71 });
	attackLeft.PushBack({ 178 - 6,852 + 6,89,71 });
	attackLeft.speedFactor = 9.0f;
	attackLeft.loop = false;

	skill.PushBack({ 0 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 89 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 178 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 267 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 356 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 445 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 534 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 0 + 5,994 + 5,89,71 + 5});
	skill.PushBack({ 0 + 5,994 + 5,89,71 + 5 });
	skill.speedFactor = 8.0f;

	deadUpRight.PushBack({ 119, 1022, 29,39 });
	deadUpRight.PushBack({ 206, 1025,37,36 });
	deadUpRight.PushBack({ 300,1031,45,30 });
	deadUpRight.speedFactor = 3.0f;
	deadUpRight.loop = false;

	deadDownRight.PushBack({386, 1030, 32, 31});
	deadDownRight.PushBack({467, 1027, 41, 34});
	deadDownRight.PushBack({564, 1015, 35, 46});
	deadDownRight.speedFactor = 3.0f;
	deadDownRight.loop = false;

	numStats = App->entities->thrallstats;

	state = states::PL_IDLE;
	anim = &idleDown;
}

bool Thrall::Update(float dt)
{
	PlayerStates(dt);

	float percentage = 1.0f;

	if (IsPlayerMoving())
		percentage = App->input->GetPercentageFromAxis();
	
	if (anim != nullptr)
		anim->speed = anim->speedFactor * percentage * dt;

	UpdateCollider();

	return true;
}

bool Thrall::Finish()
{	
	return true;
}

void Thrall::Collision(Collider* collideWith)
{
	switch (collideWith->type)
	{
		case COLLIDER_TYPE::COLLIDER_ENEMY_ATAC:
		{
			SetDamage(15, true);
			break;
		}
		case COLLIDER_TYPE::COLLIDER_UNWALKABLE:
		{
			PushOut(collideWith);
			if (state == states::PL_DASH)
				ResetDash();
			break;
		}
	}
}
	
void Thrall::UpdateCollider()
{
	if (anim == &idleUp)
	{
		pcol->colliderRect.x = 7;
		pcol->colliderRect.y = 0;
		pcol->colliderRect.w = 15;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &idleDown)
	{
		pcol->colliderRect.x = 9;
		pcol->colliderRect.y = 0;
		pcol->colliderRect.w = 15;
		pcol->colliderRect.h = 20;
	}
	else if (anim == &idleRight)
	{
		pcol->colliderRect.x = 4;
		pcol->colliderRect.y = 2;
		pcol->colliderRect.w = 13;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &idleLeft)
	{
		pcol->colliderRect.x = 4;
		pcol->colliderRect.y = 2;
		pcol->colliderRect.w = 15;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &idleUpLeft || anim == &idleUpRight || anim == &idleDownLeft || anim == &idleDownRight)
	{
		pcol->colliderRect.x = 4;
		pcol->colliderRect.y = 2;
		pcol->colliderRect.w = 15;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &upLeft)
	{
		pcol->colliderRect.x = 2;
		pcol->colliderRect.y = 2;
		pcol->colliderRect.w = 15;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &upRight)
	{
		pcol->colliderRect.x = 5;
		pcol->colliderRect.y = 0;
		pcol->colliderRect.w = 17;
		pcol->colliderRect.h = 23;
	}
	else if (anim == &up || anim == &down)
	{
		pcol->colliderRect.x = 5;
		pcol->colliderRect.y = 0;
		pcol->colliderRect.w = 17;
		pcol->colliderRect.h = 23;
	}
	
}