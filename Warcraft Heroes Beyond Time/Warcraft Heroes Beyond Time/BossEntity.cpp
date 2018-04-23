#include "BossEntity.h"

BossEntity::BossEntity(fPoint coor, BossType bossType, SDL_Texture* texture) : EnemyEntity(coor, ENEMY_TYPE::BOSS, texture), bossType(bossType) {}

bool BossEntity::Start() { return true; }

bool BossEntity::Update(float dt) { return true; }

bool BossEntity::Finish() { return true; }