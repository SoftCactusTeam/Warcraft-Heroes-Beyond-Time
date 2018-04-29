#include "ChestEntity.h"
#include "Application.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include <math.h>
#include "p2Point.h"
#include "ModuleAudio.h"
#include "ModuleColliders.h"
ChestEntity::ChestEntity(fPoint coor, CHEST_TYPE type, SDL_Texture* texture) : StaticEntity(coor, texture, StaticEntity::StaticType::CHEST), type(type)
{
	//chest.PushBack({ 1,18,32,32 });
	//chest.PushBack({ 34,18,32,32 });
	//chest.PushBack({ 67,18,32,32 });

	chest.PushBack({ 144, 16, 38, 65 });
	chest.PushBack({ 183, 16, 38, 65 });
	chest.PushBack({ 222, 16, 38, 65 });

	chest.PushBack({ 261, 16, 38, 65 });
	chest.PushBack({ 261, 16, 38, 65 });
	chest.PushBack({ 261, 16, 38, 65 });
	chest.PushBack({ 261, 16, 38, 65 });

	chest.PushBack({ 222, 16, 38, 65 });
	chest.PushBack({ 183, 16, 38, 65 });
	chest.PushBack({ 144, 16, 38, 65 });

	chest.PushBack({ 144, 82, 38, 65 });
	chest.PushBack({ 183, 82, 38, 65 });

	chest.PushBack({ 144, 16, 38, 65 });
	chest.PushBack({ 222, 82, 38, 65 });
	chest.PushBack({ 222, 82, 38, 65 });
	chest.PushBack({ 261, 82, 38, 65 });

	chest.speedFactor = 0.0f;
	chest.loop = false;
}

bool ChestEntity::Start()
{
	anim = &chest;

	col = App->colliders->AddCollider({ 9,31,20,20 }, Collider::ColliderType::ENTITY, this);
	physicalCol = App->colliders->AddCollider({ 9 + (int)pos.x, 31 + (int)pos.y, 20, 20 }, Collider::ColliderType::WALL, nullptr);

	return true;
}


bool ChestEntity::Update(float dt)
{

	anim->speed = chest.speedFactor * dt;

	
	if (SDL_RectEquals(&anim->GetCurrentRect(), &SDL_Rect({ 183, 82, 38, 65 })) && !chestSoundPlayed)
	{
		App->audio->PlayFx(App->audio->OpeningChestFX);
		chestSoundPlayed = true;
	}

	if (opened && anim->Finished() && !selection_created)
	{
		//Randomizing 3 items:
		Item* items[3] = { nullptr, nullptr, nullptr };
		App->items->getThreeRandomItems(items);
		App->scene->CreateItemSelectionScreen(items[0], items[1], items[2]);
		selection_created = true;
	}


	return true;
}

bool ChestEntity::Finish() 
{ 
	App->colliders->deleteCollider(col);
	App->colliders->deleteCollider(physicalCol);

	col = nullptr;
	physicalCol = nullptr;

	return true; 
}

void ChestEntity::OnCollision(Collider* yours, Collider* collideWith)
{
	switch (collideWith->colType)
	{
		case Collider::ColliderType::PLAYER_ATTACK:
		{
			PlayerAttack* attack = (PlayerAttack*)collideWith;
			if (!opened && attack->pattacktype == PlayerAttack::P_Attack_Type::NORMAL_ATTACK)
			{
				OpenChest();
			}
			break;
		}
	}
}

bool ChestEntity::OpenChest()
{
	
	chest.Start(12.0f);
	opened = true;
	return opened;
}

