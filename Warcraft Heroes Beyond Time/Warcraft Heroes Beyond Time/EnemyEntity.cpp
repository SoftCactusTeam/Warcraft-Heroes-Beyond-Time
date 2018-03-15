#include "EnemyEntity.h"

EnemyEntity::EnemyEntity(fPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : DynamicEntity(coor, texture), type(type) {}

bool EnemyEntity::Start() { return true; }

bool EnemyEntity::Update() { return true; }

bool EnemyEntity::Finish() { return true; }