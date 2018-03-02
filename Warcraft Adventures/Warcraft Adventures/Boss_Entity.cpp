#include "Boss_Entity.h"

Boss_Entity::Boss_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, BOSS_TYPE character) : DinamicEntity(coor, type, texture)
{
	boss_type = character;
}

void Boss_Entity::DoAction(BOSS_ACTIONS action, FIXED_ANGLE angle) {

}

void Boss_Entity::UseConsumable() {

}

void Boss_Entity::Start() {

}

void Boss_Entity::Finish() {

}

void Boss_Entity::Draw() {

}

void Boss_Entity::Update() {

}

void Boss_Entity::Move() {

}