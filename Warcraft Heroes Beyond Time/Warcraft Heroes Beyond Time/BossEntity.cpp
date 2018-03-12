#include "BossEntity.h"

BossEntity::BossEntity(fPoint coor, BOSS_TYPE type, SDL_Texture* texture) : DynamicEntity(coor, texture), type(type) {}

bool BossEntity::Start() { return true; }

bool BossEntity::Update() { return true; }

bool BossEntity::Finish() { return true; }