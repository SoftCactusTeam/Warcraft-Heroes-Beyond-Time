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
	chest.PushBack({ 1,18,32,32 });
	chest.PushBack({ 34,18,32,32 });
	chest.PushBack({ 67,18,32,32 });
	chest.speedFactor = 0.0f;
	chest.loop = false;

	
}

bool ChestEntity::Start()
{
	anim = &chest;

	col = App->colliders->AddCollider({ 0,0,20,20 }, Collider::ColliderType::ENTITY, this);

	return true;
}

bool ChestEntity::Update(float dt)
{

	anim->speed = chest.speedFactor * dt;

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
	App->audio->PlayFx(App->audio->OpeningChestFX);
	chest.Start(2.0f);
	opened = true;
	return opened;
}

