#include "Application.h"
#include "Thrall.h"
#include "ModuleInput.h"
#include "ModuleEntitySystem.h"
#include "ModuleColliders.h"
#include "ModuleAudio.h"
#include "Scene.h"
#include "WCItem.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : PlayerEntity(coor, type, texture)
{
	// Thrall idle animations

	idleUp.PushBack({ 25,15,43,41 }, { 1,0 }); //Example: Introduce here the pivot.
	idleUp.PushBack({ 114,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 203,15,43,41 }, { 1,0 });
	idleUp.PushBack({ 292,15,43,41 }, { 1,0 });
	idleUp.speedFactor = 3.0f;

	idleUpRight.PushBack({ 382,15,34,40 }, { 0,0 });
	idleUpRight.PushBack({ 471,15,34,40 }, { 0,1 });
	idleUpRight.PushBack({ 560,15,34,40 }, { 0,1 });
	idleUpRight.PushBack({ 649,15,34,40 }, { 0,0 });
	idleUpRight.speedFactor = 3.0f;

	idleRight.PushBack({ 34,86,25,44 }, { 0,0 });
	idleRight.PushBack({ 123,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 212,86,25,44 }, { 0,1 });
	idleRight.PushBack({ 301,86,25,44 }, { 0,0 });
	idleRight.speedFactor = 3.0f;

	idleDownRight.PushBack({ 389,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 477,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 566,86,24,40 }, { 0,0 });
	idleDownRight.PushBack({ 656,86,24,40 }, { 0,0 });
	idleDownRight.speedFactor = 3.0f;

	idleDown.PushBack({ 17,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 106,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 195,157,48,39 }, { 16,0 });
	idleDown.PushBack({ 284,157,48,39 }, { 16,0 });
	idleDown.speedFactor = 3.0f;

	idleDownLeft.PushBack({ 384,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 473,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 562,162,30,29 }, { 5,0 });
	idleDownLeft.PushBack({ 651,162,30,29 }, { 5,0 });
	idleDownLeft.speedFactor = 3.0f;

	idleLeft.PushBack({ 35,225,25,44 }, { 0,4 });
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

	right.PushBack({ 28,372,40,44 }, { 6, 0 });
	right.PushBack({ 118,372,40,44 }, { 6, 1 });
	right.PushBack({ 207,372,40,44 }, { 6, 0 });
	right.PushBack({ 296,372,40,44 }, { 6, 0 });
	right.speedFactor = 9.0f;

	downRight.PushBack({ 383,371,36,40 }, { 8, 1 });
	downRight.PushBack({ 472,371,36,40 }, { 8, 1 });
	downRight.PushBack({ 561,371,36,40 }, { 8, 0 });
	downRight.PushBack({ 650,371,36,40 }, { 8, 1 });
	downRight.speedFactor = 9.0f;

	down.PushBack({ 15,447,48,39 }, { 16,0 });
	down.PushBack({ 104,447,48,39 }, { 16,2 });
	down.PushBack({ 193,447,48,39 }, { 16,2 });
	down.PushBack({ 282,447,48,39 }, { 16,2 });
	down.speedFactor = 9.0f;

	downLeft.PushBack({ 377,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 466,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 555,446,41,33 }, { 10, 2 });
	downLeft.PushBack({ 644,446,41,33 }, { 10, 2 });
	downLeft.speedFactor = 9.0f;

	left.PushBack({ 20,512,40,36 }, { 10,0 });
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

	attackUp.PushBack({ 24,580,46,47 }, { 4,9 });
	attackUp.PushBack({ 114,580,46,47 }, { 4,10 });
	attackUp.PushBack({ 202,580,46,47 }, { 4,9 });
	attackUp.PushBack({ 291,580,46,47 }, { 4,8 });
	attackUp.PushBack({ 380,580,46,47 }, { 4,8 });
	attackUp.speedFactor = 15.0f;
	attackUp.loop = false;

	attackUpRight.PushBack({ 466,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 556,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 645,582,45,46 }, { 7,7 });
	attackUpRight.PushBack({ 24,652,45,46 }, { 7,7 });
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
	attackDownRight.PushBack({ 12,727,56,44 }, { 20,2 });
	attackDownRight.PushBack({ 100,727,56,44 }, { 20,2 });
	attackDownRight.PushBack({ 189,727,56,44 }, { 20,0 });
	attackDownRight.PushBack({ 279,727,56,44 }, { 20,0 });
	attackDownRight.speedFactor = 15.0f;
	attackDownRight.loop = false;

	attackDown.PushBack({ 374,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 464,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 551,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 641,717,52,54 }, { 15,14 });
	attackDown.PushBack({ 18,796,52,54 }, { 15,5 });
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

	attackUpLeft.PushBack({ 289,866,51,44 }, { 8, 10 });
	attackUpLeft.PushBack({ 381,866,51,44 }, { 8, 10 });
	attackUpLeft.PushBack({ 469,866,51,44 }, { 8, 9 });
	attackUpLeft.PushBack({ 551,866,51,44 }, { 16, 9 });
	attackUpLeft.PushBack({ 640,866,51,44 }, { 16, 9 });
	attackUpLeft.speedFactor = 15.0f;
	attackUpLeft.loop = false;

<<<<<<< HEAD
	skill.PushBack({ 0 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 89 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 178 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 267 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 356 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 445 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 534 + 5,923 + 5,89,71 + 5 });
	skill.PushBack({ 0 + 5,994 + 5,89,71 + 5 });
	skill.PushBack({ 0 + 5,994 + 5,89,71 + 5 });
=======
	skill.PushBack({ 13 ,933,61,67 }, { 16,21 });
	skill.PushBack({ 102,933,61,67 }, { 16,21 });
	skill.PushBack({ 191,933,61,67 }, { 16,21 });
	skill.PushBack({ 280,933,61,67 }, { 16,21 });
	skill.PushBack({ 370,933,61,67 }, { 15,21 });
	skill.PushBack({ 459,933,61,67 }, { 15,21 });
	skill.PushBack({ 548,933,61,67 }, { 15,21 });
	skill.PushBack({ 637,933,61,67 }, { 15,21 });
	skill.PushBack({ 13,1006,61,67 }, { 16,19 });
>>>>>>> master
	skill.speedFactor = 8.0f;

	deadUpRight.PushBack({ 119, 1022, 29,39 });
	deadUpRight.PushBack({ 206, 1025,37,36 });
	deadUpRight.PushBack({ 300,1031,45,30 });
	deadUpRight.speedFactor = 3.0f;
	deadUpRight.loop = false;

	deadDownRight.PushBack({ 386, 1030, 32, 31 });
	deadDownRight.PushBack({ 467, 1027, 41, 34 });
	deadDownRight.PushBack({ 564, 1015, 35, 46 });
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

	if (attacking)
		UpdateAttackCollider();

	else if (skillOn)
		UpdateSkillCollider();

	if (App->scene->paper != nullptr && App->scene->paper->got_paper)
	{
		time += 1 * dt;
		if (time >= 0.1)
		{
			time = 0;
			if (cont < 18)
			{
				wcpaper.push_front({ (int)App->scene->player->pos.x,(int)App->scene->player->pos.y });
				paper_collider.push_front(App->colliders->AddCollider(SDL_Rect({ (int)App->scene->player->pos.x,(int)App->scene->player->pos.y,32,32 }), COLLIDER_PLAYER_ATTACK, nullptr, {0,0}, Collider::ATTACK_TYPE::SHIT));
				cont += 1;
			}
			if (cont == 18)
			{
				cont -= 1;
				wcpaper.pop_back();
				App->colliders->deleteCollider(paper_collider.back());
				paper_collider.pop_back();
			}
			}
		std::list<iPoint>::iterator it = wcpaper.begin();
		
		for (; it != wcpaper.end(); ++it)
		{
			App->printer->PrintSprite({ it->x, it->y }, App->scene->venom, SDL_Rect({ 0,0,32,32 }), -1);
		}
	}

	return true;
}

bool Thrall::PostUpdate()
{
	if (anim == &attackUp || anim == &attackDown || anim == &attackRight || anim == &attackLeft || anim == &attackUpLeft || anim == &attackUpRight || anim == &attackDownLeft || anim == &attackDownRight)
	{
<<<<<<< HEAD
		if (anim->Finished() || attackCollider->collidingWith == COLLIDER_ENEMY)
=======
		if (anim->Finished() || (attackCollider != nullptr && attackCollider->collidingWith == COLLIDER_ENEMY))
>>>>>>> master
		{
			if ((attackCollider != nullptr && attackCollider->collidingWith == COLLIDER_ENEMY))
			{
				IncreaseEnergy(20);
				App->audio->PlayFx(App->audio->Thrall_Hit_FX);
			}
				
			attacking = false;
			App->colliders->deleteCollider(attackCollider);
			attackCollider = nullptr;
		}
	}

	if (anim == &skill)
	{
<<<<<<< HEAD
		if (anim->Finished() || skillCollider->collidingWith == COLLIDER_ENEMY)
=======
		if (anim->Finished() || (skillCollider != nullptr && skillCollider->collidingWith != COLLIDER_NONE))
>>>>>>> master
		{
			skillOn = false;
			App->colliders->deleteCollider(skillCollider);
		}
	}

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
	case COLLIDER_TYPE::COLLIDER_ENEMY_ATTACK:
	{
		if (collideWith->attackType == Collider::ATTACK_TYPE::ENEMY_ARROW)
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
	case COLLIDER_TYPE::COLLIDER_FELBALL:
	{
		SetDamage(50, true);
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
{
	if (anim == &attackUp)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 291,580,46,47 })))
		{
			attackCollider->colliderRect = { (int)pos.x - 5, (int)pos.y - 10, 38, 30 };
		}
	}
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
			attackCollider->colliderRect = { (int)pos.x - 15, (int)pos.y - 15, 40, 45 };
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
}
