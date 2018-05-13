#include "ArrowSlowItem.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Scene.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "Thrall.h"



ArrowSlowItem::ArrowSlowItem()
{
}


ArrowSlowItem::~ArrowSlowItem()
{
	std::list<Arrow_Slow_Shit*>::iterator it = arrow_slow_shit_list.begin();
	for (; it != arrow_slow_shit_list.end(); ++it)
	{
		delete *it;
	}
	arrow_slow_shit_list.clear();
}

bool ArrowSlowItem::Start()
{
	return true;
}

bool ArrowSlowItem::Act(ModuleItems::ItemEvent event, float dt)
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
				Arrow_Slow_Shit* arrow_slow_shit_pointer = new Arrow_Slow_Shit(*(App->colliders->AddPlayerAttackCollider(SDL_Rect({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y, 32, 32 }), nullptr, 0, PlayerAttack::P_Attack_Type::PROJECTILESLOWSHIT_ITEM)).lock(), iPoint({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y }));
				arrow_slow_shit_list.push_front(arrow_slow_shit_pointer);
				cont += 1;
			}
			if (cont == 18)
			{
				cont -= 1;
				std::list<Arrow_Slow_Shit*>::iterator it = arrow_slow_shit_list.end();
				std::advance(it, -1);
				delete *it;
				arrow_slow_shit_list.erase(it);

			}
		}
		break;
	}
	return true;
}

bool ArrowSlowItem::Draw()
{
	std::list<Arrow_Slow_Shit*>::iterator it = arrow_slow_shit_list.begin();

	for (; it != arrow_slow_shit_list.end(); ++it)
	{
		App->printer->PrintSprite((*it)->pos, App->items->getItemsTexture(), SDL_Rect(ARROW_SLOW_ITEM), -1);
	}


	return true;
}
	

bool ArrowSlowItem::printYourStuff(iPoint pos)
{
	iPoint iconPos = { 171 / 2 - 31 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(SHEEP_ICON), 1, 0);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 200 + pos.y });
	return true;
}

Arrow_Slow_Shit::Arrow_Slow_Shit(Collider * temp, iPoint pos) : arrow_slow_collider(temp), pos(pos)
{
}

Arrow_Slow_Shit::~Arrow_Slow_Shit()
{
	App->colliders->deleteCollider(arrow_slow_collider);
}
