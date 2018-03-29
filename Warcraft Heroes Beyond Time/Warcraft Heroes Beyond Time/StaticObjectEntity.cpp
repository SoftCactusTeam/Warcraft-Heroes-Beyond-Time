#include "StaticObjectEntity.h"

StaticObjectEntity::StaticObjectEntity(fPoint coor, SDL_Texture* texture) : StaticEntity(coor, texture) {}

bool StaticObjectEntity::Start() { return true; }

bool StaticObjectEntity::Finish() { return true; }