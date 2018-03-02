#include "Consumable_Entity.h"

Consumable_Entity::Consumable_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : StaticEntity(coor, type, texture)
{}

void Consumable_Entity::Start() {}
void Consumable_Entity::Finish() {}
void Consumable_Entity::Draw() {}

void Consumable_Entity::Interaction() {}