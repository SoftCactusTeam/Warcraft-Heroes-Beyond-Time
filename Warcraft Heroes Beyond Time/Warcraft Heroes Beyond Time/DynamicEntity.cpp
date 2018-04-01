#include "DynamicEntity.h"
#include <math.h>

DynamicEntity::DynamicEntity(fPoint coor, SDL_Texture* texture) : Entity(coor, texture) {}

bool DynamicEntity::Start() { return true; }
bool DynamicEntity::Update(float dt) { return true; }
bool DynamicEntity::Finish() { return true; }

FIXED_ANGLE DynamicEntity::CaculateAngle(fPoint objectiveCoor)
{
	FIXED_ANGLE angleToReturn = FIXED_ANGLE::NON_ANGLE;
	if (this->pos.x - objectiveCoor.x >= 0)
	{
		if (this->pos.y - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_LEFT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_LEFT;
	}
	else
	{
		if (this->pos.y - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_RIGHT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_RIGHT;
	}

	float dX = 0.0f;
	float dY = 0.0f;
	switch (angleToReturn)
	{
	case FIXED_ANGLE::UP_RIGHT:
		dX = (float)objectiveCoor.x - (float)pos.x;
		dY = (float)pos.y - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::UP_LEFT:
		dX = (float)pos.x - (float)objectiveCoor.x;
		dY = (float)pos.y - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		dX = (float)objectiveCoor.x - (float)pos.x;
		dY = (float)objectiveCoor.y - (float)pos.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		dX = (float)pos.x - (float)objectiveCoor.x;
		dY = (float)objectiveCoor.y - (float)pos.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	}

	return angleToReturn;
}

fPoint DynamicEntity::CaculateFPointAngle(fPoint objectiveCoor)
{
	switch (CaculateAngle(objectiveCoor))
	{
	case UP: return fPoint(0, -3); break;
	case UP_RIGHT: return fPoint(2, -2); break;
	case RIGHT: return fPoint(3, 0); break;
	case DOWN_RIGHT: return fPoint(2, 2); break;
	case DOWN: return fPoint(0, 3); break;
	case DOWN_LEFT: return fPoint(-2, 2); break;
	case LEFT: return fPoint(-3, 0); break;
	case UP_LEFT: return fPoint(-2, -2); break;
	}
}

uint DynamicEntity::DistanceToObejective(fPoint objectiveCoor)
{
	uint totalX = this->pos.x - objectiveCoor.x;
	uint totalY = this->pos.y - objectiveCoor.y;
	return sqrt((totalX * totalX) + (totalY * totalY));
}

void DynamicEntity::Collision(COLLIDER_TYPE type) {}