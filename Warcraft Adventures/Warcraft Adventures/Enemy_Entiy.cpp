#include "Enemy_Entity.h"

Enemy_Entity::Enemy_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, ENEMY_TYPE character) : DinamicEntity(coor, type, texture)
{
	boss_type = character;
}

void Enemy_Entity::DoAction(ENEMY_ACTIONS action, FIXED_ANGLE angle) {

}

void Enemy_Entity::UseConsumable() {

}

void Enemy_Entity::Start() {

}

void Enemy_Entity::Finish() {

}

void Enemy_Entity::Draw() {

}

void Enemy_Entity::Update() {

}

void Enemy_Entity::Move() {

}