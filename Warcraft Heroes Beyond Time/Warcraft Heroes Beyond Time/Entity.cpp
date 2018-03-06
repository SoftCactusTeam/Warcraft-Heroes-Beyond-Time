#include "App.h"
#include "Entity.h"
#include "ModuleRender.h"

Entity::Entity(iPoint coor, SDL_Texture* texture) : coor(coor), texture(texture) {}

bool Entity::Start() { return true; }

bool Entity::Update(float dt) { return true; }

bool Entity::Draw(float dt)
{
	bool ret = true;

	ret = Application->render->Blit(texture, coor.x, coor.y, &rectToPrint);

	return ret;
}

bool Entity::Finish() { return true; }