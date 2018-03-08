#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(iPoint coor, PLAYER_TYPE type, SDL_Texture* texture) : DynamicEntity (coor, texture), type(type) {}

bool PlayerEntity::Start() { return true; }

bool PlayerEntity::Update(float dt) { return true; }

bool PlayerEntity::Finish() { return true; }