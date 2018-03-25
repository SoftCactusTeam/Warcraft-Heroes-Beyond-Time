#include "StaticEntity.h"

StaticEntity::StaticEntity(fPoint coor, SDL_Texture* texture) : Entity(coor, texture) {}

bool StaticEntity::Start() { return true; }

bool StaticEntity::Update(float dt) { return true; }

bool StaticEntity::Finish() { return true; }

void StaticEntity::Collision(COLLIDER_TYPE type) {}