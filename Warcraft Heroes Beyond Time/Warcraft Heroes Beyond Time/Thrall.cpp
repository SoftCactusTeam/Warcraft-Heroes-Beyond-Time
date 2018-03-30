#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture) 
{
	idleDown.PushBack({ 0,188,55,47 });
	idleDown.PushBack({ 55,188,55,47 });
	idleDown.PushBack({ 110,188,55,47 });
	idleDown.PushBack({ 165,188,55,47 });
	idleDown.speedFactor = 3.0f;

	idleUp.PushBack({ 0,0,55,47 });
	idleUp.PushBack({ 55,0,55,47 });
	idleUp.PushBack({ 110,0,55,47 });
	idleUp.PushBack({ 165,0,55,47 });
	idleUp.speedFactor = 3.0f;

	idleLeft.PushBack({ 0,282,55,47 });
	idleLeft.PushBack({ 55,282,55,47 });
	idleLeft.PushBack({ 110,282,55,47 });
	idleLeft.PushBack({ 165,282,55,47 });
	idleLeft.speedFactor = 3.0f;

	idleRight.PushBack({ 0,94,55,47 });
	idleRight.PushBack({ 55,94,55,47 });
	idleRight.PushBack({ 110,94,55,47 });
	idleRight.PushBack({ 165,94,55,47 });
	idleRight.speedFactor = 3.0f;

	idleUpRight.PushBack({ 0,47,55,47 });
	idleUpRight.PushBack({ 55,47,55,47 });
	idleUpRight.PushBack({ 110,47,55,47 });
	idleUpRight.PushBack({ 165,47,55,47 });
	idleUpRight.speedFactor = 3.0f;

	idleUpLeft.PushBack({ 0,329,55,47 });
	idleUpLeft.PushBack({ 55,329,55,47 });
	idleUpLeft.PushBack({ 110,329,55,47 });
	idleUpLeft.PushBack({ 165,329,55,47 });
	idleUpLeft.speedFactor = 3.0f;

	idleDownRight.PushBack({ 0,141,55,47 });
	idleDownRight.PushBack({ 55,141,55,47 });
	idleDownRight.PushBack({ 110,141,55,47 });
	idleDownRight.PushBack({ 165,141,55,47 });
	idleDownRight.speedFactor = 3.0f;

	idleDownLeft.PushBack({ 0,235,55,47 });
	idleDownLeft.PushBack({ 55,235,55,47 });
	idleDownLeft.PushBack({ 110,235,55,47 });
	idleDownLeft.PushBack({ 165,235,55,47 });
	idleDownLeft.speedFactor = 3.0f;

	down.PushBack({ 0,564,55,47 });
	down.PushBack({ 55,564,55,47 });
	down.PushBack({ 110,564,55,47 });
	down.PushBack({ 165,564,55,47 });
	down.speedFactor = 9.0f;

	up.PushBack({ 0,376,55,47 });
	up.PushBack({ 55,376,55,47 });
	up.PushBack({ 110,376,55,47 });
	up.PushBack({ 165,376,55,47 });
	up.speedFactor = 9.0f;

	left.PushBack({ 0,658,55,47 });
	left.PushBack({ 55,658,55,47 });
	left.PushBack({ 110,658,55,47 });
	left.PushBack({ 165,658,55,47 });
	left.speedFactor = 9.0f;

	right.PushBack({ 0,470,55,47 });
	right.PushBack({ 55,470,55,47 });
	right.PushBack({ 110,470,55,47 });
	right.PushBack({ 165,470,55,47 });
	right.speedFactor = 9.0f;

	upRight.PushBack({ 0,423,55,47 });
	upRight.PushBack({ 55,423,55,47 });
	upRight.PushBack({ 110,423,55,47 });
	upRight.PushBack({ 165,423,55,47 });
	upRight.speedFactor = 9.0f;

	upLeft.PushBack({ 0,705,55,47 });
	upLeft.PushBack({ 55,705,55,47 });
	upLeft.PushBack({ 110,705,55,47 });
	upLeft.PushBack({ 165,705,55,47 });
	upLeft.speedFactor = 9.0f;

	downRight.PushBack({ 0,517,55,47 });
	downRight.PushBack({ 55,517,55,47 });
	downRight.PushBack({ 110,517,55,47 });
	downRight.PushBack({ 165,517,55,47 });
	downRight.speedFactor = 9.0f;

	downLeft.PushBack({ 0,611,55,47 });
	downLeft.PushBack({ 55,611,55,47 });
	downLeft.PushBack({ 110,611,55,47 });
	downLeft.PushBack({ 165,611,55,47 });
	downLeft.speedFactor = 9.0f;

	// ------- Dash animations -------

	dashRight.PushBack({ 110, 470, 55, 47 });
	dashRight.speed = 0.2f;

	dashDown.PushBack({ 110,564,55,47 });
	dashDown.speed = 0.2f;

	dashUpRight.PushBack({ 165,423,55,47 });
	dashUpRight.speed = 0.2f;

	dashDownRight.PushBack({ 165,517,55,47 });
	dashDownRight.speed = 0.2f;

	dashDownLeft.PushBack({ 165,611,55,47 });
	dashDownRight.speed = 0.2f;

	dashUp.PushBack({ 110,376,55,47 });
	dashUp.speed = 0.2f;

	dashUpLeft.PushBack({ 165,705,55,47 });
	dashUpLeft.speed = 0.2f;

	dashLeft.PushBack({ 110,658,55,47 });
	dashLeft.speed = 0.2f;

	// ---- attack animation ----

	attackDown.PushBack({ 256,309,51,54 });
	attackDown.PushBack({ 322,309,51,54 });
	attackDown.PushBack({ 398,309,51,54 });
	attackDown.PushBack({ 486,309,51,54 });
	attackDown.PushBack({ 571,309,51,54 });
	attackDown.speedFactor = 9.0f;

	attackUp.PushBack({ 263,389,46,47 });
	attackUp.PushBack({ 336,389,46,47 });
	attackUp.PushBack({ 409,389,46,47 });
	attackUp.PushBack({ 482,389,46,47 });
	attackUp.PushBack({ 555,389,46,47 });
	attackUp.speedFactor = 9.0f;

	attackRight.PushBack({ 256,558,51,36 });
	attackRight.PushBack({ 329,558,51,36 });
	attackRight.PushBack({ 402,558,51,36 });
	attackRight.PushBack({ 475,558,51,36 });
	attackRight.PushBack({ 548,558,51,36 });
	attackRight.speedFactor = 9.0f;

	attackLeft.PushBack({ 260,626,59,39 });
	attackLeft.PushBack({ 333,626,59,39 });
	attackLeft.PushBack({ 406,626,59,39 });
	attackLeft.PushBack({ 479,626,59,39 });
	attackLeft.PushBack({ 552,626,59,39 });
	attackLeft.speedFactor = 9.0f;

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