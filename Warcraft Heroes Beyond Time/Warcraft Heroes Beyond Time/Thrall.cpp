#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"
#include "ModuleEntitySystem.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture) 
{
	// Thrall idle animations

	idleUp.PushBack({ 0,0,89,71 }, {0,0}); //Example: Introduce here the pivot.
	idleUp.PushBack({ 89,0,89,71 });
	idleUp.PushBack({ 178,0,89,71 });
	idleUp.PushBack({ 267,0,89,71 });
	idleUp.speedFactor = 3.0f;

	idleUpRight.PushBack({ 356,0,89,71 });
	idleUpRight.PushBack({ 445,0,89,71 });
	idleUpRight.PushBack({ 534,0,89,71 });
	idleUpRight.PushBack({ 623,0,89,71 });
	idleUpRight.speedFactor = 3.0f;

	idleRight.PushBack({ 0,71,89,71 });
	idleRight.PushBack({ 89,71,89,71 });
	idleRight.PushBack({ 178,71,89,71 });
	idleRight.PushBack({ 267,71,89,71 });
	idleRight.speedFactor = 3.0f;

	idleDownRight.PushBack({ 356,71,89,71 });
	idleDownRight.PushBack({ 445,71,89,71 });
	idleDownRight.PushBack({ 534,71,89,71 });
	idleDownRight.PushBack({ 623,71,89,71 });
	idleDownRight.speedFactor = 3.0f;

	idleDown.PushBack({ 0 + 9,142,89,71 });
	idleDown.PushBack({ 89 + 9,142,89,71 });
	idleDown.PushBack({ 178 + 9,142,89,71 });
	idleDown.PushBack({ 267 + 9,142,89,71 });
	idleDown.speedFactor = 3.0f;

	idleDownLeft.PushBack({ 356,142,89,71 });
	idleDownLeft.PushBack({ 445,142,89,71 });
	idleDownLeft.PushBack({ 534,142,89,71 });
	idleDownLeft.PushBack({ 623,142,89,71 });
	idleDownLeft.speedFactor = 3.0f;

	idleLeft.PushBack({ 0,213,89,71 });
	idleLeft.PushBack({ 89,213,89,71 });
	idleLeft.PushBack({ 178,213,89,71 });
	idleLeft.PushBack({ 267,213,89,71 });
	idleLeft.speedFactor = 3.0f;

	// TODO
	/*idleUpLeft.PushBack({ 356,213,89,71 });
	idleUpLeft.PushBack({ 445,213,89,71 });
	idleUpLeft.PushBack({ 534,213,89,71 });
	idleUpLeft.PushBack({ 623,213,89,71 });
	idleUpLeft.speedFactor = 3.0f;*/

	// Thrall walk animations

	up.PushBack({ 0,284,89,71 });
	up.PushBack({ 89,284,89,71 });
	up.PushBack({ 178,284,89,71 });
	up.PushBack({ 267,284,89,71 });
	up.speedFactor = 9.0f;

	upRight.PushBack({ 356,284,89,71 });
	upRight.PushBack({ 445,284,89,71 });
	upRight.PushBack({ 534,284,89,71 });
	upRight.PushBack({ 623,284,89,71 });
	upRight.speedFactor = 9.0f;

	right.PushBack({ 0,355,89,71 });
	right.PushBack({ 89,355,89,71 });
	right.PushBack({ 178,355,89,71 });
	right.PushBack({ 267,355,89,71 });
	right.speedFactor = 9.0f;

	downRight.PushBack({ 356,355,89,71 });
	downRight.PushBack({ 445,355,89,71 });
	downRight.PushBack({ 534,355,89,71 });
	downRight.PushBack({ 623,355,89,71 });
	downRight.speedFactor = 9.0f;

	down.PushBack({ 0,426,89,71 });
	down.PushBack({ 89,426,89,71 });
	down.PushBack({ 178,426,89,71 });
	down.PushBack({ 267,426,89,71 });
	down.speedFactor = 9.0f;

	downLeft.PushBack({ 356,426,89,71 });
	downLeft.PushBack({ 445,426,89,71 });
	downLeft.PushBack({ 534,426,89,71 });
	downLeft.PushBack({ 623,426,89,71 });
	downLeft.speedFactor = 9.0f;

	left.PushBack({ 0,497,89,71 });
	left.PushBack({ 89,497,89,71 });
	left.PushBack({ 178,497,89,71 });
	left.PushBack({ 267,497,89,71 });
	left.speedFactor = 9.0f;

	upLeft.PushBack({ 356,497,89,71 });
	upLeft.PushBack({ 445,497,89,71 });
	upLeft.PushBack({ 534,497,89,71 });
	upLeft.PushBack({ 623,497,89,71 });
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

	damagedAnim.PushBack({ 664,612,47,44 });
	damagedAnim.PushBack({ 718,612,47,44 });
	damagedAnim.PushBack({ 773,612,47,56 });
	damagedAnim.speedFactor = 9.0f;

	live = 100;

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