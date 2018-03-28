#include "PortalEntity.h"
#include "StaticEntity.h"

PortalEntity::PortalEntity(fPoint coor, STATIC_ENTITY_TYPE type, SDL_Texture * texture) : StaticEntity(coor, texture), type(type)
{
	portal.PushBack({ 310,9,131,129 });
	portal.PushBack({ 310,139,131,129 });
}

bool PortalEntity::Start()
{
	anim = &portal;
	return true;
}

bool PortalEntity::Update(float dt)
{
	return true;
}

bool PortalEntity::Finish()
{
	return true;
}
