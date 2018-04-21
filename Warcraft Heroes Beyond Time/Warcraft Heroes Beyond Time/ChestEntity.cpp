#include "ChestEntity.h"
#include "Application.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include <math.h>
#include "p2Point.h"
#include "ModuleAudio.h"
#include "ModuleColliders.h"
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

	//Create the collider

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

void ChestEntity::OnCollision(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
		case Collider::ColliderType::PLAYER_ATTACK:
		{
			if (!opened)
			{
				OpenChest();
			}
			break;
		}
	}
}

bool ChestEntity::OpenChest()
{
	App->audio->PlayFx(App->audio->OpeningChestFX);
	chest.Start(2.0f);
	opened = true;

	//Randomizing 3 items:
	Item* items[3] = { nullptr, nullptr, nullptr };
	App->items->getThreeRandomItems(items);



	return opened;
}

