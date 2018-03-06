#include "StaticEntity.h"

StaticEntity::StaticEntity(iPoint coor, SDL_Texture* texture) : Entity(coor, texture) {}

bool StaticEntity::Start() { return true; }

bool StaticEntity::Update(float dt) { return true; }

bool StaticEntity::Finish() { return true; }