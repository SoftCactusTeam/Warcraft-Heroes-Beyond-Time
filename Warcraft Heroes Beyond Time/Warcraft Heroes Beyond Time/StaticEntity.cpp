#include "StaticEntity.h"

StaticEntity::StaticEntity(fPoint coor, SDL_Texture* texture, StaticEntity::StaticType staticType) : Entity(coor, texture, EntityType::STATIC_ENTITY), staticType(staticType){}

bool StaticEntity::Start() { return true; }

bool StaticEntity::Update(float dt) { return true; }

bool StaticEntity::Finish() { return true; }