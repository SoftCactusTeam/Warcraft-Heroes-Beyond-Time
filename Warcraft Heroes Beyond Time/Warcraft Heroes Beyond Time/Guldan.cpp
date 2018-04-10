#include "BossEntity.h"
#include "Guldan.h"
#include <time.h>

Guldan::Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture) : BossEntity(coor, type, texture)
{
	idle.PushBack({ 1,1,68,68 });
	idle.PushBack({ 70,1,68,68 });
	idle.PushBack({ 139,1,68,68 });
	idle.PushBack({ 208,1,68,68 });
	idle.PushBack({ 277,1,68,68 });
	idle.speedFactor = 9.0f;

	teleport.PushBack({ 484,73,68,68 });
	teleport.PushBack({ 554,73,68,68 });
	teleport.PushBack({ 623,73,68,68 });
	teleport.PushBack({ 697,73,68,68 });
	teleport.PushBack({ 697,73,68,68 });
	teleport.PushBack({ 697,73,68,68 });
	teleport.PushBack({ 697,73,68,68 });
	teleport.PushBack({ 697,73,68,68 });

	live = 1000;
	anim = &idle;
}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	effectsTexture = App->textures->Load("sprites/Guldan_Effects.png");

	srand(time(NULL));
	statesBoss = BossStates::IDLE;

	CreateFelBalls({ pos.x, pos.y });

	for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
	{
		(*it)->StartMovement();
	}

	return true;
}

bool Guldan::Update(float dt)
{

	switch (statesBoss)
	{
		case BossStates::IDLE:
		{

			if (live == 1000)
			{
				statesBoss = BossStates::TELEPORT;
				break;
			}

			break;
		}
		case BossStates::TELEPORT:
		{

			//if anim current frame == tal
				//tpPoints[rand() % 5];
			//if current == end
				//state == idle

			break;
		}

	}

	anim->speed = anim->speedFactor * dt;

	if (createNewBalls)
	{
		createNewBalls = false;
		fellBallsList.clear();
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
			createNewBalls = true;
		}
		
	}	

	return true;
}

bool Guldan::Finish()
{
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
