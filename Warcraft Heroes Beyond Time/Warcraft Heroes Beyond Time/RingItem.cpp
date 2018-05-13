#include "RingItem.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Scene.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "Thrall.h"


RingItem::RingItem()
{
}


RingItem::~RingItem()
{
}

bool RingItem::Start()
{
	return true;
}

bool RingItem::Act(ModuleItems::ItemEvent event, float dt)
{
	switch (event)
	{
		case ModuleItems::ItemEvent::UPDATE:
			time += dt;
			if (time >= 0.1)
			{
				time = 0;
				if (cont < 18)
				{
					Slow_Shit* slow_shit_pointer = new Slow_Shit(*App->colliders->AddPlayerAttackCollider(SDL_Rect({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y, 32, 32 }), nullptr, 0, PlayerAttack::P_Attack_Type::ENEMYSLOWSHIT_ITEM).lock(), iPoint({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y }));
					slow_shit_list.push_front(slow_shit_pointer);
					cont += 1;
				}
				if (cont == 18)
				{
					cont -= 1;
					std::list<Slow_Shit*>::iterator it = slow_shit_list.end();
					std::advance(it, -1);
					delete *it;
					slow_shit_list.erase(it);

				}
			}
			break;
	}

	return true;
}

bool RingItem::Draw()
{
	std::list<Slow_Shit*>::iterator it = slow_shit_list.begin();

	for (; it != slow_shit_list.end(); ++it)
	{
		App->printer->PrintSprite((*it)->pos, App->items->getItemsTexture(), SDL_Rect(RING_ITEM), -1);
	}
	return true;
}

bool RingItem::printYourStuff(iPoint pos)
{
	iPoint iconPos = { 171 / 2 - 19 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(RING_ICON), 1, 0);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 200 + pos.y });
	return true;
}

Slow_Shit::Slow_Shit(Collider* temp, iPoint pos) : slow_shit_collider(temp), pos(pos)
{
}

Slow_Shit::~Slow_Shit()
{
	App->colliders->deleteCollider(slow_shit_collider);
}
