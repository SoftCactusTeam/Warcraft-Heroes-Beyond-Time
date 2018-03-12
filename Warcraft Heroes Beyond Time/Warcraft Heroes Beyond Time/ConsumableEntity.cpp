#include "ConsumableEntity.h"

ConsumableEntity::ConsumableEntity(fPoint coor, CONSUMABLE_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type) {}

bool ConsumableEntity::Start() { return true; }

bool ConsumableEntity::Finish() { return true; }