#include "ChestEntity.h"

ChestEntiy::ChestEntiy(iPoint coor, CHEST_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type) {}

bool ChestEntiy::Start() { return true; }

bool ChestEntiy::Finish() { return true; }