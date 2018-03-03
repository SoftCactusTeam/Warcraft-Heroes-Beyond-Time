#include "StaticObject_Entity.h"

StaticObject_Entity::StaticObject_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, STATIC_OBJECT_TYPE staticObjectType) : StaticEntity(coor, type, texture)
{
	this->staticObjectType = staticObjectType;
}

void StaticObject_Entity::Start() {}
void StaticObject_Entity::Finish() {}

void StaticObject_Entity::Interaction() {}