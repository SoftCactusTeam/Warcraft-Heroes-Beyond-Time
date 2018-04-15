#include "BossEntity.h"
#include "Guldan.h"
#include <time.h>
#include "ModuleInput.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "Thrall.h"

Guldan::Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture) : BossEntity(coor, type, texture)
{
	idle.PushBack({ 1,1,68,68 });
	idle.PushBack({ 70,1,68,68 });
	idle.PushBack({ 139,1,68,68 });
	idle.PushBack({ 208,1,68,68 });
	idle.PushBack({ 277,1,68,68 });
	idle.speedFactor = 9.0f;

	teleport.PushBack({ 484,70 ,68,68 });
	teleport.PushBack({ 553,70 ,68,68 });
	teleport.PushBack({ 622,70 ,68,68 });
	teleport.PushBack({ 691,70 ,68,68 });
	teleport.PushBack({ 760,70 ,68,68 });
	teleport.PushBack({ 829,70 ,68,68 });
	teleport.PushBack({ 898,70 ,68,68 });
	teleport.PushBack({ 1  ,139,68,68 });
	teleport.PushBack({ 70 ,139,68,68 });
	teleport.PushBack({ 139,139,68,68 });
	teleport.PushBack({ 208,139,68,68 });
	teleport.PushBack({ 277,139,68,68 });
	teleport.PushBack({ 346,139,68,68 });
	teleport.PushBack({ 415,139,68,68 });
	teleport.PushBack({ 484,139,68,68 });
	teleport.PushBack({ 553,139,68,68 });
	teleport.PushBack({ 622,139,68,68 });
	teleport.PushBack({ 691,139,68,68 });
	teleport.PushBack({ 760,139,68,68 });
	teleport.speedFactor = 9.0f;
	teleport.loop = false;

	inverseTeleport.PushBack({ 760,139,68,68 });
	inverseTeleport.PushBack({ 691,139,68,68 });
	inverseTeleport.PushBack({ 622,139,68,68 });
	inverseTeleport.PushBack({ 553,139,68,68 });
	inverseTeleport.PushBack({ 484,139,68,68 });
	inverseTeleport.PushBack({ 415,139,68,68 });
	inverseTeleport.PushBack({ 346,139,68,68 });
	inverseTeleport.PushBack({ 277,139,68,68 });
	inverseTeleport.PushBack({ 208,139,68,68 });
	inverseTeleport.PushBack({ 139,139,68,68 });
	inverseTeleport.PushBack({ 70 ,139,68,68 });
	inverseTeleport.PushBack({ 1  ,139,68,68 });
	inverseTeleport.PushBack({ 898,70 ,68,68 });
	inverseTeleport.PushBack({ 829,70 ,68,68 });
	inverseTeleport.PushBack({ 760,70 ,68,68 });
	inverseTeleport.PushBack({ 691,70 ,68,68 });
	inverseTeleport.PushBack({ 622,70 ,68,68 });
	inverseTeleport.PushBack({ 553,70 ,68,68 });
	inverseTeleport.PushBack({ 484,70 ,68,68 });
	inverseTeleport.speedFactor = 9.0f;
	inverseTeleport.loop = false;

	dead.PushBack({ 2,283,60,64 });
	dead.PushBack({ 71,283,60,64 });
	dead.PushBack({ 140,283,60,64 });
	dead.PushBack({ 209,283,60,64 });
	dead.PushBack({ 278,283,60,64 });
	dead.PushBack({ 347,283,60,64 });
	dead.PushBack({ 416,283,60,64 });
	dead.PushBack({ 487,283,60,64 });
	dead.PushBack({ 555,283,60,64 });
	dead.PushBack({ 624,283,60,64 });
	dead.PushBack({ 691,283,60,64 });
	dead.PushBack({ 760,283,60,64 });
	dead.PushBack({ 829,283,60,64 });
	dead.PushBack({ 898,283,60,64 });
	dead.loop = false;
	dead.speedFactor = 9.0f;

	generateingBalls.PushBack({ 484,1,68,68 });
	generateingBalls.PushBack({ 553,1,68,68 });
	generateingBalls.PushBack({ 623,1,68,68 });
	generateingBalls.PushBack({ 692,1,68,68 });
	generateingBalls.PushBack({ 760,1,68,68 });
	generateingBalls.PushBack({ 830,1,68,68 });
	generateingBalls.PushBack({ 898,1,68,68 });
	generateingBalls.PushBack({ 830,1,68,68 });
	generateingBalls.PushBack({ 898,1,68,68 });
	generateingBalls.loop = false;		
	generateingBalls.speedFactor = 9.0f;


	generatingBallsInverse.PushBack({ 898,1,68,68 });
	generatingBallsInverse.PushBack({ 830,1,68,68 });
	generatingBallsInverse.PushBack({ 898,1,68,68 });
	generatingBallsInverse.PushBack({ 830,1,68,68 });
	generatingBallsInverse.PushBack({ 760,1,68,68 });
	generatingBallsInverse.PushBack({ 692,1,68,68 });
	generatingBallsInverse.PushBack({ 623,1,68,68 });
	generatingBallsInverse.PushBack({ 553,1,68,68 });
	generatingBallsInverse.PushBack({ 484,1,68,68 });
	generatingBallsInverse.loop = false;
	generatingBallsInverse.speedFactor = 9.0f;

	hello.PushBack({ 208,71,69,68 });
	hello.PushBack({ 276,71,68,68 });
	hello.PushBack({ 346,71,68,68 });
	hello.PushBack({ 415,71,68,68 });
	hello.PushBack({ 346,71,68,68 });
	hello.PushBack({ 276,71,68,68 });
	hello.PushBack({ 208,71,68,68 });
	hello.loop = false;
	hello.speedFactor = 9.0f;

	anim = &idle;

	numStats.hp = 1000;
	numStats.maxhp = 1000;

	isGuldan = true;
}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	bossCol = App->colliders->AddCollider({ 5, 5,55,55 }, COLLIDER_TYPE::COLLIDER_ENEMY, this);

	effectsTexture = App->textures->Load("sprites/Guldan_Effects.png");

	srand(time(NULL));
	statesBoss = BossStates::IDLE;

	return true;
}

bool Guldan::Update(float dt)
{
	/*if (soundBalls)
	{
		timerBalls += 1.0f * dt;

		if (timerBalls >= 8.0f)
		{
			App->audio->PlayFx(App->audio->GuldanFireBallFX);
			timerBalls = 0.0f;
			soundBalls = false;
		}
	}*/
	if (fellBallsList.size() <= 0)
		ballsOnTheAir = false;
	else
		ballsOnTheAir = true;

	if (firstEncounter)
	{
		if (startTimeForTP)
		{
			fellBallsList.clear();
			floatTimeForTp += 1.0f + dt;

		}

		if (readeForTimeNewBalls)
		{
			timeForNewBalls += 1.0f * dt;
			if (timeForNewBalls >= 2.0f)
			{
				readeForTimeNewBalls = false;
				readyToTP = false;
				startTimeForTP = false;
				timeForNewBalls = 0.0f;
				createNewBalls = true;
			}
		}

		switch (statesBoss)
		{
		case BossStates::IDLE:
		{
			if (numStats.hp <= 0)
			{
				App->scene->player->Win();
				anim = &dead;
				floatTimeForTp = 0.0f;
				startTimeForTP = 0.0f;
				createNewBalls = false;
				readeForTimeNewBalls = false;
				statesBoss = BossStates::DEAD;
				App->audio->PlayFx(App->audio->GuldanDieFX);
				if (bossCol != nullptr)
				{
					App->colliders->deleteCollider(bossCol);
					bossCol = nullptr;
				}
				break;
			}

			if (floatTimeForTp >= 2.0f)
			{
				App->audio->PlayFx(App->audio->GuldanTPFX);
				bossCol->colliderRect = { 0,0,0,0 };
				statesBoss = BossStates::TELEPORT;
				anim = &teleport;
				floatTimeForTp = 0.0f;
				startTimeForTP = false;
				break;
			}

			if (createNewBalls)
			{
				soundBalls = true;
				anim = &generateingBalls;
				createNewBalls = false;
				statesBoss = BossStates::GENERATINGBALLS;
				App->audio->PlayFx(App->audio->GuldanBalls_Ori);
				break;
			}

			break;
		}
		case BossStates::TELEPORT:
		{
			if (anim->Finished())
			{
				App->audio->PlayFx(App->audio->GuldanTPFX);
				int randomtp;
				createNewBalls = false;
				do
				{
					randomtp = rand() % 5;
				} while (tpPoints[randomtp].x == (int)pos.x / 48 && tpPoints[randomtp].y == (int)pos.y / 48);
				pos.x = tpPoints[randomtp].x * 48;
				pos.y = tpPoints[randomtp].y * 48;
				anim = &inverseTeleport;
				teleport.Reset();;
				statesBoss = BossStates::INVERSETELEPORT;
				break;
			}

			break;
		}

		case BossStates::INVERSETELEPORT:
		{

			if (anim->Finished())
			{
				anim = &idle;
				statesBoss = BossStates::IDLE;
				readeForTimeNewBalls = true;
				inverseTeleport.Reset();
				bossCol->colliderRect = { 5, 5,55,55 };
				break;
			}

			break;
		}

		case BossStates::DEAD:
		{
			for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
			{
				if ((*it)->dead)
				{
					delete *it;
					startTimeForTP = true;
				}

			}
			fellBallsList.clear();

			break;
		}

		case BossStates::GENERATINGBALLS:

			if (numStats.hp <= 0)
			{
				App->scene->player->Win();
				anim = &dead;
				floatTimeForTp = 0.0f;
				startTimeForTP = 0.0f;
				createNewBalls = false;
				readeForTimeNewBalls = false;
				statesBoss = BossStates::DEAD;
				App->audio->PlayFx(App->audio->GuldanDieFX);
				if (bossCol != nullptr)
				{
					App->colliders->deleteCollider(bossCol);
					bossCol = nullptr;
				}
				break;
			}

			if (anim == &generateingBalls)
			{
				if (anim->Finished())
				{
					anim->Reset();
					anim = &generatingBallsInverse;
					readyforfornewballs = true;
					break;
				}
			}
			else if (anim == &generatingBallsInverse)
			{
				if (anim->Finished())
				{
					anim->Reset();
					anim = &idle;
					statesBoss = BossStates::IDLE;
					break;
				}

			}
			break;
		}

		if (readyforfornewballs)
		{
			createNewBalls = false;
			readyforfornewballs = false;
			CreateFelBalls({ pos.x, pos.y });
			for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
			{
				(*it)->StartMovement();
			}
		}


		for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
		{
			(*it)->Update(dt);
			(*it)->BlitFel();
		}


		for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
		{
			if ((*it)->dead)
			{
				delete *it;
				startTimeForTP = true;
			}

		}
	}
	else
	{
		if (pos.DistanceTo(App->scene->player->pos) < 140.0f)
		{
			anim = &hello;
		}
		if (anim == &hello)
		{
			if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 208, 71, 69, 68 })))
			{
				App->audio->PlayFx(App->audio->GuldanEncounterFX);
				App->input->PlayJoyRumble(0.95f, 500);
			}
			if (anim->Finished())
			{
				firstEncounter = true;
				anim = &idle;
				startTimeForTP = true;
			}
		}
	}

	anim->speed = anim->speedFactor * dt;
	return true;
}

bool Guldan::Finish()
{
	for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
	{
		if ((*it)->dead)
		{
			delete *it;
			startTimeForTP = true;
		}

	}
	if (bossCol != nullptr)
	{
		App->colliders->deleteCollider(bossCol);
		bossCol = nullptr;
	}
	App->textures->UnLoad(effectsTexture);
	return true;
}

bool Guldan::CreateFelBalls(fPoint pos)
{
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 0, effectsTexture, 90));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 45, effectsTexture, 90));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 90, effectsTexture, 90));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 135, effectsTexture, 90));
																				 
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 180, effectsTexture, 90));
																				 
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 225, effectsTexture, 90));
																				  
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 270, effectsTexture, 90));
																									 																					 
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 315, effectsTexture, 90));

	return true;
}

void Guldan::Collision(Collider* collideWith)
{
	switch (collideWith->type)
	{
		case COLLIDER_TYPE::COLLIDER_PLAYER_ATTACK:
		{
			if (collideWith->attackType == Collider::ATTACK_TYPE::PLAYER_MELEE)
			{
				if (anim == &idle || anim == &generateingBalls || anim == &generatingBallsInverse)
				{
					if (numStats.hp - 10 <= 0)
						numStats.hp = 0;
					else
						numStats.hp -= 10;
				}
			}
			else if (collideWith->attackType == Collider::ATTACK_TYPE::THRALL_SKILL)
			{
				if (anim == &idle || anim == &generateingBalls || anim == &generatingBallsInverse)
				{
					if ((int)numStats.hp - 10 <= 0)
						numStats.hp = 0;
					else
						numStats.hp -= 10;
				}
			}
			else if (collideWith->attackType == Collider::ATTACK_TYPE::SHIT && !App->scene->player->stop)
			{
				if (numStats.hp -  (1 * App->dt) <= 0)
					numStats.hp = 0;
				else
					numStats.hp -= 1 * App->dt;
			}
			break;
		}
	}
}
