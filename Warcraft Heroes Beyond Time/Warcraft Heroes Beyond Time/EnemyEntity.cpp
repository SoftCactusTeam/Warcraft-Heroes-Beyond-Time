#include "EnemyEntity.h"

EnemyEntity::EnemyEntity(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : DynamicEntity(coor, texture), type(type) {}

bool EnemyEntity::Start() { return true; }

bool EnemyEntity::Update(float dt) { return true; }

bool EnemyEntity::Finish() { return true; }