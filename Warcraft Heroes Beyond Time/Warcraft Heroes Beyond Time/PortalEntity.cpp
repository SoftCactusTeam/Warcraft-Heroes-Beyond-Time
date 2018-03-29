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
	if (SDL_RectEquals(&anim->GetCurrentFrame(), &SDL_Rect({ 310, 139, 131, 129 })))
		anim->speedFactor = 0.0f;

	anim->speed = anim->speedFactor * dt;
	return true;
}

bool PortalEntity::Finish()
{
	return true;
}

void PortalEntity::OpenPortal()
{
	portal.speedFactor = 1.0f;
}
