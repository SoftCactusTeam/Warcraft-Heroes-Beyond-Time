#include "Chest_Entity.h"

Chest_Entiy::Chest_Entiy(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture) : StaticEntity(coor, type, texture)
{}

void Chest_Entiy::Start() {}
void Chest_Entiy::Finish() {}
void Chest_Entiy::Draw() {}

void Chest_Entiy::Interaction() {}