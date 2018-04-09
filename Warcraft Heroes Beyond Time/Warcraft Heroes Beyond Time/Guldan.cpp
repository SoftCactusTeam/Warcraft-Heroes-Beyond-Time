#include "BossEntity.h"
#include "Guldan.h"

#include <time.h>

Guldan::Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture) : BossEntity(coor, type, texture)
{
	idle.PushBack({ 0,0,89,71 });

	live = 1000;
	anim = &idle;
}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	srand(time(NULL));
	statesBoss = BossStates::IDLE;

	CreateFelBall({ pos.x, pos.y });

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

	for (std::list<FelBall*>::const_iterator it = fellBallsList.begin(); it != fellBallsList.end(); ++it)
	{
		(*it)->Update(dt);
		(*it)->BlitFel();
	}

	if (fellBallsList.size() <= 0)
	{
		CreateFelBall({ pos.x, pos.y });
	}

	return true;
}

bool Guldan::Finish()
{
	return true;
}

bool Guldan::CreateFelBall(fPoint pos)
{
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 0));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 45));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 90));

	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 135));
														
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 180));
														
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 225));
														
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 270));
														
	fellBallsList.push_back(new FelBall({ pos.x + 23 / 2, pos.y + 23 / 2 }, 60, 315));

	return true;
}
