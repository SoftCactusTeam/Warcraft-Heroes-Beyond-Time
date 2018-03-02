#include "Chest_Entity.h"

Chest_Entiy::Chest_Entiy(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, CHEST_TYPE chestType) : StaticEntity(coor, type, texture)
{
	this->chestType = chestType;
}

void Chest_Entiy::Start() {}
void Chest_Entiy::Finish() {}
void Chest_Entiy::Draw() {}

void Chest_Entiy::Interaction() {}