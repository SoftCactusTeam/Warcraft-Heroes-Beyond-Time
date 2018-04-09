#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"
#include "ModuleEntitySystem.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture)
{
	// Thrall idle animations

	idleUp.PushBack({ 25,15,43,41 },  { 1,0 }); //Example: Introduce here the pivot.
	idleUp.PushBack({ 114,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 203,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 292,15,43,41 }, { 1,0 });
	idleUp.speedFactor = 3.0f;

	idleUpRight.PushBack({ 382,15,34,37 }, { 11,10 });
	idleUpRight.PushBack({ 471,16,34,38 }, { 11,10 });
	idleUpRight.PushBack({ 560,16,34,38 }, { 11,10 });
	idleUpRight.PushBack({ 649,15,34,37 }, { 11,10 });
	idleUpRight.speedFactor = 3.0f;

	idleRight.PushBack({ 34,86,25,44 },  { 0,0 });
	idleRight.PushBack({ 123,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 212,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 301,86,25,44 }, { 0,0 });
	idleRight.speedFactor = 3.0f;

	idleDownRight.PushBack({ 389,86,23,38 }, { 13,15 });
	idleDownRight.PushBack({ 477,87,24,38 }, { 15,15 });
	idleDownRight.PushBack({ 566,87,24,38 }, { 16,15 });
	idleDownRight.PushBack({ 656,86,23,38 }, { 14,16 });
	idleDownRight.speedFactor = 3.0f;

	idleDown.PushBack({ 17,157,48,39 },  { 16,0 });
	idleDown.PushBack({ 106,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 195,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 284,157,48,39 }, { 16,0 });
	idleDown.speedFactor = 3.0f;

	idleDownLeft.PushBack({ 384,162,30,29 }, { 15,14 });
	idleDownLeft.PushBack({ 473,163,31,30 }, { 15,14 });
	idleDownLeft.PushBack({ 562,163,31,30 }, { 15,15 });
	idleDownLeft.PushBack({ 651,162,30,29 }, { 14,14 });
	idleDownLeft.speedFactor = 3.0f;

	idleLeft.PushBack({ 35,225,25,44 },  { 0,4 });
	idleLeft.PushBack({ 124,225,25,44 }, { 0,5 });
	idleLeft.PushBack({ 213,225,25,44 }, { 0,5 });
	idleLeft.PushBack({ 302,225,25,44 }, { 0,4 });
	idleLeft.speedFactor = 3.0f;


	idleUpLeft.PushBack({ 387,233,26,34 }, { 15,11 });
	idleUpLeft.PushBack({ 476,234,26,34 }, { 15,11 });
	idleUpLeft.PushBack({ 565,234,26,34 }, { 15,11 });
	idleUpLeft.PushBack({ 654,233,26,34 }, { 15,11 });
	idleUpLeft.speedFactor = 3.0f;


	// Thrall walk animations
	up.PushBack({ 30,298,43,41 }, { 0,0 });
	up.PushBack({ 118,298,43,41 }, { 0,1 });
	up.PushBack({ 206,298,43,41 }, { 0,1 });
	up.PushBack({ 294,298,43,41 }, { 0,0 });
	up.speedFactor = 9.0f;

	upRight.PushBack({ 380,301,47,41 }, { 4,0 });
	upRight.PushBack({ 469,301,47,41 }, { 3,1 });
	upRight.PushBack({ 558,301,47,41 }, { 0,1 });
	upRight.PushBack({ 647,301,47,41 }, { 4,0 });
	upRight.speedFactor = 9.0f;

	right.PushBack({ 28,372,40,44 },  { 6, 0 });
	right.PushBack({ 118,372,40,44 }, { 6, 1 });
	right.PushBack({ 207,372,40,44 }, { 6, 0 });
	right.PushBack({ 296,372,40,44 }, { 6, 0 });
	right.speedFactor = 9.0f;

	downRight.PushBack({ 383,372,36,32 }, {23,16});
	downRight.PushBack({ 474,372,34,35 }, {23,17});
	downRight.PushBack({ 568,371,25,36 }, {18,17});
	downRight.PushBack({ 656,372,29,34 }, {19,16});
	downRight.speedFactor = 9.0f;

	down.PushBack({ 15,447,48,39 }, {16,0});
	down.PushBack({ 104,447,48,39 }, {16,2});
	down.PushBack({ 193,447,48,39 }, {16,2});
	down.PushBack({ 282,447,48,39 }, {16,2});
	down.speedFactor = 9.0f;

	downLeft.PushBack({ 389,446,25,30 }, {8,17});
	downLeft.PushBack({ 473,447,31,28 }, {13,15});
	downLeft.PushBack({ 555,447,41,32 }, {20,15});
	downLeft.PushBack({ 644,447,39,33 }, {20,16});
	downLeft.speedFactor = 9.0f;

	left.PushBack({ 20,512,40,36 },  { 10,0 });
	left.PushBack({ 109,512,40,36 }, { 10,0 });
	left.PushBack({ 198,512,40,36 }, { 10,0 });
	left.PushBack({ 287,512,40,36 }, { 10,0 });
	left.speedFactor = 9.0f;

	upLeft.PushBack({ 379,514,49,35 }, {20,10});
	upLeft.PushBack({ 472,513,43,36 }, {16,11});
	upLeft.PushBack({ 564,513,26,32 }, {13,10});
	upLeft.PushBack({ 652,514,28,30 }, {15,8});
	upLeft.speedFactor = 9.0f;

	//

	// ------- Dash animations -------

	dashUp.PushBack({ 267,284,89,71 });
	dashUp.speed = 0.2f;

	dashUpRight.PushBack({ 534,284,89,71 });
	dashUpRight.speed = 0.2f;

	dashRight.PushBack({ 267,355,89,71 });
	dashRight.speed = 0.2f;

	dashDownRight.PushBack({ 534,355,89,71 });
	dashDownRight.speed = 0.2f;

	dashDown.PushBack({ 178,426,89,71 });
	dashDown.speed = 0.2f;

	dashDownLeft.PushBack({ 534,426,89,71 });
	dashDownRight.speed = 0.2f;

	dashLeft.PushBack({ 178,497,89,71 });
	dashLeft.speed = 0.2f;

	dashUpLeft.PushBack({ 534,497,89,71 });
	dashUpLeft.speed = 0.2f;

	// ---- attack animation ----

	attackUp.PushBack({ 0 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 89 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 178 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 267 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 356 + 3,568 + 6,89,71 });
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

	return true;
}

bool Thrall::Finish()
{	
	return true;
}

void Thrall::Collision(COLLIDER_TYPE type)
{
	switch (type)
	{
	case COLLIDER_TYPE::COLLIDER_ENEMY_ATAC:
		printf_s("ha rebut una hostia guapa");
		break;
	}
}