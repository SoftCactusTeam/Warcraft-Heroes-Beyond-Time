#include "ChestEntity.h"
#include "Application.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include <math.h>
#include "p2Point.h"


ChestEntity::ChestEntity(fPoint coor, CHEST_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture), type(type)
{
	chest.PushBack({ 1,18,32,32 });
	chest.PushBack({ 34,18,32,32 });
	chest.PushBack({ 67,18,32,32 });
	chest.speedFactor = 0.0f;
}

bool ChestEntity::Start()
{
	chest.Stop();
	anim = &chest;
	return true;
}

bool ChestEntity::Update(float dt)
{
	if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({67,18,32,32})))
		chest.Stop();
	
		anim->speed = chest.speedFactor * dt;

	return true;
}

bool ChestEntity::Finish() { return true; }

bool ChestEntity::OpenChest()
{
	if (!locked)
		chest.Start(2.0f);
	
	return !locked;
}

void ChestEntity::UnLockChest()
{
	locked = false;
}

inline bool ChestEntity::IsLocked() const
{
	return locked;
}

bool ChestEntity::PlayerNear(fPoint pos)
{
	if (App->scene->lvlChest != nullptr)
	{
		float norm;
		float result = App->scene->lvlChest->pos.x - pos.x;
		float result2 = result * result;
		float result3 = App->scene->lvlChest->pos.y - pos.y;
		float result4 = result3 * result3;
		float result5 = result2 + result4;
		norm = sqrt(result5);
		if (norm <= 40)
			return true;
		else
			return false;
	}
	else
		return false;
}