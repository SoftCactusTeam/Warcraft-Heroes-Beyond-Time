#include "BossEntity.h"
#include "Guldan.h"

Guldan::Guldan(fPoint coor, BOSS_TYPE type, SDL_Texture* texture) : BossEntity(coor, type, texture)
{

}

Guldan::~Guldan()
{
}

bool Guldan::Start()
{
	statesBoss = BossStates::IDLE;

	return true;
}

bool Guldan::Update(float dt)
{

	switch (statesBoss)
	{
		case BossStates::IDLE:
		{


			break;
		}

	}

	return true;
}

bool Guldan::Finish()
{
	return true;
}
