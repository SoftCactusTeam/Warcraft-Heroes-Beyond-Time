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
#include "PortalEntity.h"
#include "ModuleItems.h"
#include "ModuleEffects.h"
#include "EnemyEntity.h"
#include "Guldan.h"

Thrall::Thrall(fPoint coor, PLAYER_TYPE type, SDL_Texture* texture, EntitySystem::PlayerStats& numStats) : PlayerEntity(coor, type, texture)
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

	dashUp.PushBack({ 12,1724,37,45 }, { 0,1 });
	dashUp.speed = 0.2f;

	dashUpRight.PushBack({ 53,1723,51,45 }, { 5,1 });
	dashUpRight.speed = 0.2f;

	dashRight.PushBack({ 106,1724,51,45 }, { 6, 0 });
	dashRight.speed = 0.2f;

	dashDownRight.PushBack({ 161,1725,36,45 }, { 8, 0 });
	dashDownRight.speed = 0.2f;

	dashDown.PushBack({ 208,1725,41,45 }, { 16,2 });
	dashDown.speed = 0.2f;

	dashDownLeft.PushBack({ 250,1725,48,45 }, { 10, 2 });
	dashDownRight.speed = 0.2f;

	dashLeft.PushBack({ 300,1730,46,40 }, { 10,0 });
	dashLeft.speed = 0.2f;

	dashUpLeft.PushBack({ 348,1730,35,40 }, { 9,0 });
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

	skill.PushBack({ 0  ,928 ,200,200 }, { 87,92 });
	skill.PushBack({ 0  ,1128,200,200 }, { 87,92 });
	skill.PushBack({ 0  ,1328,200,200 }, { 87,92 });
	skill.PushBack({ 0  ,1528,200,200 }, { 87,92 });
	skill.PushBack({ 200,928 ,200,200 }, { 87,92 });
	skill.PushBack({ 200,1128,200,200 }, { 87,92 });
	skill.PushBack({ 200,1328,200,200 }, { 87,92 });
	skill.PushBack({ 200,1528,200,200 }, { 87,92 });
	skill.PushBack({ 400,928 ,200,200 }, { 87,92 });
	skill.PushBack({ 400,1128,200,200 }, { 87,92 });
	skill.PushBack({ 400,1328,200,200 }, { 87,92 });
	skill.PushBack({ 400,1528,200,200 }, { 87,92 });
	skill.speedFactor = 9.0f;
	skill.loop = false;

	deadUpRight.PushBack({ 650, 942, 29,39 });
	deadUpRight.PushBack({ 651, 1004,37,36 });
	deadUpRight.PushBack({ 649,1068,45,30 });
	deadUpRight.speedFactor = 3.0f;
	deadUpRight.loop = false;

	deadDownRight.PushBack({ 653, 1125, 32, 31 });
	deadDownRight.PushBack({ 647, 1185, 41, 34 });
	deadDownRight.PushBack({ 660, 1243, 35, 46 });
	deadDownRight.speedFactor = 3.0f;
	deadDownRight.loop = false;

	if (numStats.isEmpty())
		this->numStats = App->entities->thrallstats;
	else
		this->numStats = numStats;

	wallCol = *App->colliders->AddCollider({ 7, 0, 15, 23 }, Collider::ColliderType::ENTITY, this).lock();
	damageCol = *App->colliders->AddCollider({ 7, 0, 15, 23 }, Collider::ColliderType::ENTITY, this).lock();

	deadDownRight.Reset();

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

	return true;
}

bool Thrall::PostUpdate()
{
	if (anim == &attackUp || anim == &attackDown || anim == &attackRight || anim == &attackLeft || anim == &attackUpLeft || anim == &attackUpRight || anim == &attackDownLeft || anim == &attackDownRight)
	{
		if (anim->Finished())
		{
			attacking = false;
			App->colliders->deleteCollider(attackCollider);
			attackCollider = nullptr;
		}
	}
	
	else if (anim == &skill)
	{
		if (anim->Finished())
		{
			skillOn = false;
			App->colliders->deleteCollider(skillCollider);
		}
	}

	else
	{
		if (state != states::PL_ATTACK && (attacking || attackCollider))
		{
			attacking = false;
			App->colliders->deleteCollider(attackCollider);
			attackCollider = nullptr;
		}
	}

	return true;
}

bool Thrall::Finish()
{
	if (dashEmitter != nullptr)
	{
		App->psystem->RemoveEmitter(dashEmitter);
		dashEmitter = nullptr;
	}
	return true;
}

void Thrall::OnCollision(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
		case Collider::ColliderType::ENEMY_ATTACK:
		{
			if (yours == damageCol)
			{
				EnemyAttack* attack = (EnemyAttack*)collideWith;
				int a = attack->damage;
				if (state != states::PL_DASH && state != states::PL_SKILL && godMode == false)
				{
					SetDamage(attack->damage, true);
					App->items->newEvent(ModuleItems::ItemEvent::PLAYER_HITTED);
				}
			}
			
			break;
		}
		case Collider::ColliderType::WALL:
		{
			if (yours == wallCol)
			{
				PushOut(collideWith);
				if (state == states::PL_DASH)
				{
					//ResetDash();
					App->audio->HaltFX(App->audio->Thrall_Dash_FX);
				}
			}
			break;
		}
		case Collider::ColliderType::PORTAL:
		{
			if (yours == this->wallCol && App->scene->portal->locked == false)
			{
				App->scene->GoNextLevel();
			}
			break;
		}
		case Collider::ColliderType::ENTITY:
		{
			Entity* entityOwner = (Entity*)collideWith->owner;
			if(entityOwner->entityType == Entity::EntityType::DYNAMIC_ENTITY)
				if (yours->colType == Collider::ColliderType::PLAYER_ATTACK && yours == this->attackCollider)
				{
					PlayerAttack* attack = (PlayerAttack*)yours;
					if (entityOwner->entityType == Entity::EntityType::DYNAMIC_ENTITY)
					{
						DynamicEntity* dynOwner = (DynamicEntity*)entityOwner;
						if (dynOwner->dynamicType == DynamicEntity::DynamicType::ENEMY)
						{
							EnemyEntity* enemyOwner = (EnemyEntity*)dynOwner;
							if (enemyOwner->enemytype == ENEMY_TYPE::BOSS)
							{
								Guldan* guldanOwner = (Guldan*)enemyOwner;

								if (guldanOwner->statesBoss == Guldan::BossStates::TELEPORT)
									break;
							}
						}
					}
						

					if (attack->pattacktype == PlayerAttack::P_Attack_Type::NORMAL_ATTACK)
					{
						IncreaseEnergy(numStats.energyPercentbyHit);
						hit_state = rand() % 3 + 1;
						switch (hit_state)
						{
						case Thrall::HIT_1:
							App->audio->PlayFx(App->audio->Thrall_Hit1_FX);
							break;
						case Thrall::HIT_2:
							App->audio->PlayFx(App->audio->Thrall_Hit2_FX);
							break;
						case Thrall::HIT_3:
							App->audio->PlayFx(App->audio->Thrall_Hit3_FX);
							break;
						default:
							break;
						}
						App->items->newEvent(ModuleItems::ItemEvent::PLAYER_HIT);
					}
				}	
			break;
		}
	}
}

void Thrall::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
		case Collider::ColliderType::WALL:
		{
			if (yours == wallCol)
			{
				PushOut(collideWith);
				if (state == states::PL_DASH)
				{
					//ResetDash();
					App->audio->HaltFX(App->audio->Thrall_Dash_FX);
				}
			}
			break;
		}
	}
}


void Thrall::UpdateCollider()
{
	if (anim == &idleUp)
	{
		wallCol->rectArea.x = 7;
		wallCol->rectArea.y = 10;
		wallCol->rectArea.w = 15;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 7;
		damageCol->rectArea.y = 0;
		damageCol->rectArea.w = 15;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &idleDown)
	{
		wallCol->rectArea.x = 9;
		wallCol->rectArea.y = 0;
		wallCol->rectArea.w = 15;
		wallCol->rectArea.h = 20;

		damageCol->rectArea.x = 9;
		damageCol->rectArea.y = 0;
		damageCol->rectArea.w = 15;
		damageCol->rectArea.h = 20;
	}
	else if (anim == &idleRight)
	{
		wallCol->rectArea.x = 4;
		wallCol->rectArea.y = 2;
		wallCol->rectArea.w = 13;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 4;
		damageCol->rectArea.y = 2;
		damageCol->rectArea.w = 13;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &idleLeft)
	{
		wallCol->rectArea.x = 4;
		wallCol->rectArea.y = 2;
		wallCol->rectArea.w = 15;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 4;
		damageCol->rectArea.y = 2;
		damageCol->rectArea.w = 15;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &idleUpLeft || anim == &idleUpRight || anim == &idleDownLeft || anim == &idleDownRight)
	{
		wallCol->rectArea.x = 4;
		wallCol->rectArea.y = 2;
		wallCol->rectArea.w = 15;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 4;
		damageCol->rectArea.y = 2;
		damageCol->rectArea.w = 15;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &upLeft)
	{
		wallCol->rectArea.x = 2;
		wallCol->rectArea.y = 2;
		wallCol->rectArea.w = 15;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 2;
		damageCol->rectArea.y = 2;
		damageCol->rectArea.w = 15;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &upRight)
	{
		wallCol->rectArea.x = 5;
		wallCol->rectArea.y = 10;
		wallCol->rectArea.w = 17;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 5;
		damageCol->rectArea.y = 0;
		damageCol->rectArea.w = 17;
		damageCol->rectArea.h = 23;
	}
	else if (anim == &up || anim == &down)
	{
		wallCol->rectArea.x = 5;

		if (anim == &up)
			wallCol->rectArea.y = 10;
		else
			wallCol->rectArea.y = 0;
	
		wallCol->rectArea.w = 17;
		wallCol->rectArea.h = 23;

		damageCol->rectArea.x = 5;
		damageCol->rectArea.y = 0;
		damageCol->rectArea.w = 17;
		damageCol->rectArea.h = 23;
	}

}

void Thrall::Attack()
{
	if (!attacking)
		attack_thrall_state = rand() % 3 + 1;
	switch (attack_thrall_state)
	{
	case Thrall::ATTACK_STATE_1:
		App->audio->PlayFx(App->audio->Thrall_Attack_1_FX);
		break;
	case Thrall::ATTACK_STATE_2:
		App->audio->PlayFx(App->audio->Thrall_Attack_2_FX);
		break;
	case Thrall::ATTACK_STATE_3:
		App->audio->PlayFx(App->audio->Thrall_Attack_3_FX);
		break;
	default:
		break;
	}
	attacking = true;
	attackCollider = *App->colliders->AddPlayerAttackCollider({ -1000000000, -1000000000,20,20 }, this, numStats.damage, PlayerAttack::P_Attack_Type::NORMAL_ATTACK).lock();
}

void Thrall::UpdateAttackCollider()
{
	if (anim == &attackUp)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 291,580,46,47 })))
		{
			attackCollider->rectArea = {  - 5,  - 10, 38, 30 };
		}
	}
	else if (anim == &attackDown)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 641,717,52,54 })))
		{
			attackCollider->rectArea = {  - 15,  + 10, 40, 30 };
		}
	}
	else if (anim == &attackRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 466,658,50,36 })))
		{
			attackCollider->rectArea = { 0,  + 10, 40, 30 };
		}
	}
	else if (anim == &attackLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 102,868,54,39 })))
		{
			attackCollider->rectArea = {  - 18,  - 10, 23, 35 };
		}
	}
	else if (anim == &attackUpLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 551,866,51,44 })))
		{
			attackCollider->rectArea = {  - 18,  - 10, 50, 35 };
		}
	}
	else if (anim == &attackUpRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 24,652,45,46 })))
		{
			attackCollider->rectArea = { 0,  - 10, 40, 35 };
		}
	}
	else if (anim == &attackDownLeft)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 377,790,48,50 })))
		{
			attackCollider->rectArea = {  - 15,  - 15, 40, 45 };
		}
	}
	else if (anim == &attackDownRight)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 189,727,56,44 })))
		{
			attackCollider->rectArea = { 0, 0, 40, 45 };
		}
	}
}

void Thrall::UseSkill()
{
	if (skillOn == false)
		App->audio->PlayFx(App->audio->Thrall_SkillFX);
	skillOn = true;
	skillCollider = *App->colliders->AddPlayerAttackCollider({ -1000000000, -1000000000, 20, 20 }, this, numStats.damage * numStats.skillMultiplier, PlayerAttack::P_Attack_Type::SKILL).lock();
}

void Thrall::UpdateSkillCollider()
{
	if (anim == &skill)
	{
		if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 0,928,200,200 })))
		{
			skillCollider->rectArea = { -70 - 15, -70 - 15, 200,200 };
		}
	}
}


