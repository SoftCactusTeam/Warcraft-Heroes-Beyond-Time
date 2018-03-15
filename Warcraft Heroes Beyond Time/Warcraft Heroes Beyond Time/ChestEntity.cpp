#include "ChestEntity.h"

ChestEntiy::ChestEntiy(fPoint coor, CHEST_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type) {}

bool ChestEntiy::Start() { return true; }

bool ChestEntiy::Finish() { return true; }