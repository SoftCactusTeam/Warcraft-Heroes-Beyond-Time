#include "StaticObject_Entity.h"

StaticObject_Entity::StaticObject_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : StaticEntity(coor, type, texture)
{}

void StaticObject_Entity::Start() {}
void StaticObject_Entity::Finish() {}
void StaticObject_Entity::Draw() {}

void StaticObject_Entity::Interaction() {}