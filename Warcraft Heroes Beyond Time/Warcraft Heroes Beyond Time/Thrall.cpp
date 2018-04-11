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

<<<<<<< HEAD
	attackUp.PushBack({ 0 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 89 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 178 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 267 + 3,568 + 6,89,71 });
	attackUp.PushBack({ 356 + 3,568 + 6,89,71 });
	attackUp.speedFactor = 9.0f;

	attackRight.PushBack({ 178,639,89,71 });
	attackRight.PushBack({ 267,639,89,71 });
	attackRight.PushBack({ 356,639,89,71 });
	attackRight.PushBack({ 445,639,89,71 });
	attackRight.PushBack({ 534,639,89,71 });
	attackRight.speedFactor = 9.0f;

	attackDown.PushBack({ 356 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 445 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 534 + 8,710 + 5,89,71 });
	attackDown.PushBack({ 623 + 11,710 + 5,89,71 });
	attackDown.PushBack({ 0 + 8,781 + 5,89,71 });
	attackDown.speedFactor = 9.0f;

	attackLeft.PushBack({ 534 - 6,781 + 6,89,71 });
	attackLeft.PushBack({ 623 - 6,781 + 6,89,71 });
	//attackLeft.PushBack({ 0 ,852 + 6,89,71 });
	attackLeft.PushBack({ 89 - 6, 852 + 6,89,71 });
	attackLeft.PushBack({ 178 - 6,852 + 6,89,71 });
	attackLeft.speedFactor = 9.0f;
=======
	attackUp.PushBack({ 24,580,46,47 },  { 4,9 });
	attackUp.PushBack({ 114,580,46,47 }, { 4,10});
	attackUp.PushBack({ 202,580,46,47 }, { 4,9 });
	attackUp.PushBack({ 291,580,46,47 }, { 4,8 });
	attackUp.PushBack({ 380,580,46,47 }, { 4,8 });
	attackUp.speedFactor = 15.0f;
	attackUp.loop = false;

	attackUpRight.PushBack({ 466,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 556,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 645,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 24,652,45,46  }, { 7,7 });
	attackUpRight.PushBack({ 113,652,45,46 }, { 7,7 });
	attackUpRight.speedFactor = 15.0f;
	attackUpRight.loop = false;

	attackRight.PushBack({ 198,658,50,36 }, { 14,0 });
	attackRight.PushBack({ 287,658,50,36 }, { 14,1 });
	attackRight.PushBack({ 376,658,50,36 }, { 14,0 });
	attackRight.PushBack({ 466,658,50,36 }, { 14,0 }); 
	attackRight.PushBack({ 555,658,50,36 }, { 14,0 });
	attackRight.speedFactor = 15.0f;
	attackRight.loop = false;

	attackDownRight.PushBack({ 635,657,56,44 }, { 20,2 });
	attackDownRight.PushBack({ 12,727,56,44  }, { 20,2 });
	attackDownRight.PushBack({ 100,727,56,44 }, { 20,2 });
	attackDownRight.PushBack({ 189,727,56,44 }, { 20,0 });
	attackDownRight.PushBack({ 279,727,56,44 }, { 20,0 });
	attackDownRight.speedFactor = 15.0f;
	attackDownRight.loop = false;

	attackDown.PushBack({ 374,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 464,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 551,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 641,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 18,796,52,54 }, { 15,5});
	attackDown.speedFactor = 15.0f;
	attackDown.loop = false;

	attackDownLeft.PushBack({ 112,790,48,50 }, { 15,18 });
	attackDownLeft.PushBack({ 201,790,48,50 }, { 15,18 });
	attackDownLeft.PushBack({ 290,790,48,50 }, { 15,18 });
	attackDownLeft.PushBack({ 377,790,48,50 }, { 15,18 });
	attackDownLeft.PushBack({ 466,790,48,50 }, { 15,18 });
	attackDownLeft.speedFactor = 15.0f;
	attackDownLeft.loop = false;

	attackLeft.PushBack({ 552,797,54,39 }, { 13,7 });
	attackLeft.PushBack({ 641,797,54,39 }, { 13,7 });
	attackLeft.PushBack({ 15 ,868,54,39 }, { 13,7 });
	attackLeft.PushBack({ 102,868,54,39 }, { 17,7 });
	attackLeft.PushBack({ 191,868,54,39 }, { 17,7 });
	attackLeft.speedFactor = 15.0f;
	attackLeft.loop = false;

	attackUpLeft.PushBack({ 289,866,51,44 }, { 8, 10});
	attackUpLeft.PushBack({ 381,866,51,44 }, { 8, 10});
	attackUpLeft.PushBack({ 469,866,51,44 }, { 8, 9});
	attackUpLeft.PushBack({ 551,866,51,44 }, { 16, 9});
	attackUpLeft.PushBack({ 640,866,51,44 }, { 16, 9});
	attackUpLeft.speedFactor = 15.0f;
	attackUpLeft.loop = false;
>>>>>>> master

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
}

bool Thrall::Update(float dt)
{
	PlayerStates(dt);

	float percentage = 1.0f;

	if (IsPlayerMoving())
		percentage = App->input->GetPercentageFromAxis();
	
	if (anim != nullptr)
	anim->speed = anim->speedFactor * percentage * dt;

<<<<<<< HEAD
=======
	if (attacking)
		UpdateAttackCollider();

	else if (skillOn)
		UpdateSkillCollider();

	return true;
}

bool Thrall::PostUpdate()
{
	if (anim == &attackUp || anim == &attackDown || anim == &attackRight || anim == &attackLeft || anim == &attackUpLeft || anim == &attackUpRight || anim == &attackDownLeft || anim == &attackDownRight)
	{
		if (anim->Finished() || attackCollider->collidingWith != COLLIDER_NONE)
		{
			attacking = false;
			App->colliders->deleteCollider(attackCollider);
		}
	}

	else if (anim == &skill)
	{
		if (anim->Finished() || skillCollider->collidingWith != COLLIDER_NONE)
		{
			skillOn = false;
			App->colliders->deleteCollider(skillCollider);
		}
	}

>>>>>>> master
	return true;
}

bool Thrall::Finish()
{	
	return true;
}

<<<<<<< HEAD
void Thrall::Collision(COLLIDER_TYPE type)
=======
void Thrall::Collision(Collider* collideWith)
{
	switch (collideWith->type)
	{
		case COLLIDER_TYPE::COLLIDER_ENEMY_ATTACK:
		{
			if(collideWith->attackType == Collider::ATTACK_TYPE::ENEMY_ARROW)
				SetDamage(30, true);
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

void Thrall::Attack()
{
	attacking = true;
	attackCollider = App->colliders->AddCollider({ -10, -10,20,20 }, COLLIDER_PLAYER_ATTACK, nullptr, { 0,0 }, Collider::ATTACK_TYPE::PLAYER_MELEE);
}

void Thrall::UpdateAttackCollider()
>>>>>>> master
{
	switch (type)
	{
	case COLLIDER_TYPE::COLLIDER_ENEMY_ATAC:
		printf_s("ha rebut una hostia guapa");
		break;
	}
<<<<<<< HEAD
=======
	else if (anim == &attackDown)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 641,717,52,54 })))
		{
			attackCollider->colliderRect = { (int)pos.x - 15, (int)pos.y + 10, 40, 30 };
		}
	}
	else if (anim == &attackRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 466,658,50,36 })))
		{
			attackCollider->colliderRect = { (int)pos.x, (int)pos.y + 10, 40, 30 };
		}
	}
	else if (anim == &attackLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 102,868,54,39 })))
		{
			attackCollider->colliderRect = { (int)pos.x - 18, (int)pos.y - 10, 23, 35 };
		}
	}
	else if (anim == &attackUpLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 551,866,51,44 })))
		{
			attackCollider->colliderRect = { (int)pos.x - 18, (int)pos.y - 10, 50, 35 };
		}
	}
	else if (anim == &attackUpRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 24,652,45,46 })))
		{
			attackCollider->colliderRect = { (int)pos.x, (int)pos.y - 10, 40, 35 };
		}
	}
	else if (anim == &attackDownLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 377,790,48,50 })))
		{
			attackCollider->colliderRect = { (int)pos.x-15, (int)pos.y-15, 40, 45 };
		}
	}
	else if (anim == &attackDownRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 189,727,56,44 })))
		{
			attackCollider->colliderRect = { (int)pos.x, (int)pos.y, 40, 45 };
		}
	}
	
}

void Thrall::UseSkill()
{
	skillOn = true;
	skillCollider = App->colliders->AddCollider({ -100, -100, 5, 5 }, COLLIDER_PLAYER_ATTACK, nullptr, { 0,0 }, Collider::ATTACK_TYPE::THRALL_SKILL);
}

void Thrall::UpdateSkillCollider()
{
	if (anim == &skill)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 445 + 5,923 + 5,89,71 + 5 })))
		{
			skillCollider->colliderRect = { (int)pos.x + 10, (int)pos.y + 10, 30,30 };
		}

		else if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 445 + 5,923 + 5,89,71 + 5 })))
		{
			skillCollider->colliderRect = { (int)pos.x + 10, (int)pos.y + 10, 30,30 };
		}

		else if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 445 + 5,923 + 5,89,71 + 5 })))
		{
			skillCollider->colliderRect = { (int)pos.x + 10, (int)pos.y + 10, 30,30 };
		}

		else if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 445 + 5,923 + 5,89,71 + 5 })))
		{
			skillCollider->colliderRect = { (int)pos.x + 10, (int)pos.y + 10, 30,30 };
		}
	}
>>>>>>> master
}