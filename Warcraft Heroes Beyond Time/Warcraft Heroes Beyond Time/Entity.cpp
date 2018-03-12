#include "Application.h"
#include "Entity.h"
#include "ModuleRender.h"

Entity::Entity(iPoint coor, SDL_Texture* texture) : pos(coor), texture(texture) {}

bool Entity::Start() { return true; }

bool Entity::Update(float dt) { return true; }

bool Entity::Draw(float dt)
{
	bool ret = true;

	ret = Application->render->Blit(texture, pos.x, pos.y, &anim->GetCurrentFrame());

	return ret;
}

bool Entity::Finish() { return true; }