#include "ChestEntity.h"

ChestEntity::ChestEntity(fPoint coor, CHEST_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type)
{
	chest.PushBack({ 1,18,32,32 });
	chest.PushBack({ 34,18,32,32 });
	chest.PushBack({ 67,18,32,32 });
	chest.speed = 1.0f;
}

bool ChestEntity::Start()
{
	chest.Stop();
	anim = &chest;
	return true;
}

bool ChestEntity::Update(float dt)
{
	if (chest.Finished())
		chest.Stop();

	chest.speed *= dt;

	return true;
}

bool ChestEntity::Finish() { return true; }

bool ChestEntity::OpenChest()
{
	chest.Start();

	return true;
}

void ChestEntity::UnLockChest()
{
	locked = false;
}

inline bool ChestEntity::IsLocked() const
{
	return locked;
}
