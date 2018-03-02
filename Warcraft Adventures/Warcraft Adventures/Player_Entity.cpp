#include "Player_Entity.h"

PlayerPJ_Entity::PlayerPJ_Entity(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, PLAYER_TYPE character) : DinamicEntity (coor, type, texture)
{
	characterPJ_type = character;
}

void PlayerPJ_Entity::DoAction(PLAYER_ACTIONS action, FIXED_ANGLE angle) {

}

void PlayerPJ_Entity::UseConsumable() {

}

void PlayerPJ_Entity::Start() {

}

void PlayerPJ_Entity::Finish() {

}

void PlayerPJ_Entity::Draw() {

}

void PlayerPJ_Entity::Update() {

}

void PlayerPJ_Entity::Move() {

}