#include "Player_Entity.h"

PlayerPJ::PlayerPJ(iPoint coor, ENTITY_TYPE type, SDL_Texture* texture, PLAYER_TYPE character) : DinamicEntity (coor, type, texture)
{
	character_name = character;
}

void PlayerPJ::DoAction(PLAYER_ACTIONS action, FIXED_ANGLE angle) {

}

void PlayerPJ::UseConsumable() {

}

void PlayerPJ::Start() {

}

void PlayerPJ::Finish() {

}

void PlayerPJ::Draw() {

}

void PlayerPJ::Update() {

}

void PlayerPJ::Move() {

}