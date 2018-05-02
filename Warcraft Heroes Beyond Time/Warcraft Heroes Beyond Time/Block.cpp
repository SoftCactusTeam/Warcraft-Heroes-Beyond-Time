#include "Block.h"

#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

#include "Scene.h"
#include "PlayerEntity.h"

Block::Block(const BlockInfo* info, Projectile_type type) : Projectile(info, type)
{
	toData = (BlockInfo*)info;

	projCollider = *App->colliders->AddCollider({(int)toData->pos.x,(int)toData->pos.y,48,48},Collider::ColliderType::WALL).lock();
}

Block::~Block()
{
	App->colliders->deleteCollider(projCollider);
	RELEASE(data);
}

bool Block::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Block::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 26,(int)data->pos.y - 24 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), { 445,235,48,48 }, data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}
