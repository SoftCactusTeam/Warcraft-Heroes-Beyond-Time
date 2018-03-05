#include "Entity.h"
#include "App.h"
#include "ModuleRender.h"

// ----------------------------------- ENTITY FUNCTIONS -----------------------------------

Entity::Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : coor(coor), entityType(type), texture(texture) 
{

}

void Entity::Draw() {
	Application->render->Blit(texture, coor.x, coor.y, &rectToPrint);
}

// ----------------------------------- DINAMIC FUNCTIONS ----------------------------------

DynamicEntity::DynamicEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : Entity(coor, type, texture) {}

FIXED_ANGLE DynamicEntity::CaculateAngle(iPoint objectiveCoor)
{
	FIXED_ANGLE angleToReturn;
	if (this->coor.x - objectiveCoor.x >= 0)
	{
		if (this->coor.y - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_LEFT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_LEFT;
	}
	else
	{
		if (this->coor.y - objectiveCoor.y >= 0)
			angleToReturn = FIXED_ANGLE::UP_RIGHT;
		else
			angleToReturn = FIXED_ANGLE::DOWN_RIGHT;
	}

	float dX = 0.0f;
	float dY = 0.0f;
	switch (angleToReturn) 
	{
	case FIXED_ANGLE::UP_RIGHT:
		dX = (float)objectiveCoor.x - (float)coor.x;
		dY = (float)coor.y - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		// there could be an else angleToReturn = UpRight, but isn't necessary
		break;
	case FIXED_ANGLE::UP_LEFT:
		dX = (float)coor.x - (float)objectiveCoor.x;
		dY = (float)coor.y - (float)objectiveCoor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::UP;
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		dX = (float)objectiveCoor.x - (float)coor.x;
		dY = (float)objectiveCoor.y - (float)coor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::RIGHT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		dX = (float)coor.x - (float)objectiveCoor.x;
		dY = (float)objectiveCoor.y - (float)coor.y;
		if (dX / 2.5f > dY)
			angleToReturn = FIXED_ANGLE::LEFT;
		else if (dY / 2.5f > dX)
			angleToReturn = FIXED_ANGLE::DOWN;
		break;
	}

	return angleToReturn;
}

// ----------------------------------- STATIC FUNCTIONS -----------------------------------

StaticEntity::StaticEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : Entity(coor, type, texture) {}