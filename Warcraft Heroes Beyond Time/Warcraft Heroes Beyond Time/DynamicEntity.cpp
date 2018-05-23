#include "DynamicEntity.h"
#include <math.h>

DynamicEntity::DynamicEntity(fPoint coor, SDL_Texture* texture, DynamicEntity::DynamicType dynamicType) : Entity(coor, texture, Entity::EntityType::DYNAMIC_ENTITY), dynamicType(dynamicType){}

bool DynamicEntity::Start() { return true; }
bool DynamicEntity::Update(float dt) { return true; }
bool DynamicEntity::Finish() { return true; }

FIXED_ANGLE DynamicEntity::CaculateAngle(fPoint objectiveCoor, int width, int height)
{
	float pX = pos.x + width;
	float pY = pos.y + height;

	FIXED_ANGLE angleToReturn = FIXED_ANGLE::NON_ANGLE;
	if (pX - objectiveCoor.x >= 0)
	{
		if (pY - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_LEFT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_LEFT;
	}
	else
	{
		if (pY - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_RIGHT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_RIGHT;
	}

	float dX = 0.0f;
	float dY = 0.0f;
	switch (angleToReturn)
	{
	case FIXED_ANGLE::UP_RIGHT:
		dX = (float)objectiveCoor.x - pX;
		dY = pY - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::UP_LEFT:
		dX = pX - (float)objectiveCoor.x;
		dY = pY - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		dX = (float)objectiveCoor.x - pX;
		dY = (float)objectiveCoor.y - pY;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		dX = pX - (float)objectiveCoor.x;
		dY = (float)objectiveCoor.y - pY;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	}

	return angleToReturn;
}

fPoint DynamicEntity::CaculateFPointAngle(fPoint objectiveCoor, int width, int height)
{
	switch (CaculateAngle(objectiveCoor, width, height))
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
	uint totalX = (pos.x + anim->GetCurrentRect().w / 2 ) - objectiveCoor.x;
	uint totalY = (pos.y + anim->GetCurrentRect().h / 2 ) - objectiveCoor.y;
	return sqrt((totalX * totalX) + (totalY * totalY));
}

fPoint DynamicEntity::transformFixedAngleTofPoint(FIXED_ANGLE angle)
{
	switch (angle)
	{
	case UP: return fPoint(0, -3); break;
	case UP_RIGHT: return fPoint(2, -2); break;
	case RIGHT: return fPoint(3, 0); break;
	case DOWN_RIGHT: return fPoint(2, 2); break;
	case DOWN: return fPoint(0, 3); break;
	case DOWN_LEFT: return fPoint(-2, 2); break;
	case LEFT: return fPoint(-3, 0); break;
	case UP_LEFT: return fPoint(-2, -2); break;
	default: return fPoint(0,0); break;
	}
}