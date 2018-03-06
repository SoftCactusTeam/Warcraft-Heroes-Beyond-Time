#include "StaticObjectEntity.h"

StaticObjectEntity::StaticObjectEntity(iPoint coor, STATIC_OBJECT_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type) {}

bool StaticObjectEntity::Start() { return true; }

bool StaticObjectEntity::Finish() { return true; }