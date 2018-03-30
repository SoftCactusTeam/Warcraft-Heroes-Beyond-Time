#include "Application.h"
#include "Entity.h"
#include "ModuleRender.h"
#include "ModulePrinter.h"

Entity::Entity(fPoint coor, SDL_Texture* texture) : pos(coor), texture(texture) {}

bool Entity::Start() { return true; }

bool Entity::Update(float dt) { return true; }

bool Entity::Draw(float dt)
{
	bool ret = true;

	ret = App->printer->PrintSprite(iPoint(pos.x, pos.y), texture, anim->GetCurrentFrame());

	return ret;
}

bool Entity::Finish() { return true; }

void Entity::Collision(COLLIDER_TYPE type) {}