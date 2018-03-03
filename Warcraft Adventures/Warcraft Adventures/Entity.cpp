#include "Entity.h"
#include "App.h"
#include "ModuleRender.h"

Entity::Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) {
	this->coor = coor;
	this->entityType = type;
	this->texture = texture;

	destroy = false;
}

void Entity::Draw() {
	Application->render->Blit(texture, coor.x, coor.y, &rectToPrint);
}



DinamicEntity::DinamicEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : Entity(coor, type, texture)
{}

StaticEntity::StaticEntity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : Entity(coor, type, texture)
{}

FIXED_ANGLE DinamicEntity::CaculateAngle(iPoint objectiveCoor)
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

	switch (angleToReturn) 
	{
	case FIXED_ANGLE::UP_RIGHT:
		//float dX = (float)objectiveCoor.x - (float)coor.x;
		//float dY = (float)coor.y - (float)objectiveCoor.y;
		//if (dX / 2.5f > dY)
		//	angleToReturn = FIXED_ANGLE::RIGHT;
		//else if (dY / 2.5f > dX)
		//	angleToReturn = FIXED_ANGLE::UP;
		// there could be an else angleToReturn = UpRight, but isn't necessary
		break;
	case FIXED_ANGLE::UP_LEFT:

		break;
	case FIXED_ANGLE::DOWN_RIGHT:

		break;
	case FIXED_ANGLE::DOWN_LEFT:

		break;
	}

	return angleToReturn;
}

void DinamicEntity::ReceivAtac(int damage, FIXED_ANGLE angle) {
	live -= damage;
	if (live <= 0)
		Finish();
	else {	// AQUI POSAREM LA DISTANCIA QUE TEMPUTXEN AL REBRE UN COP

	}
}