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
				Arrow_Slow_Shit* arrow_slow_shit_pointer = new Arrow_Slow_Shit(App->colliders->AddPlayerAttackCollider(SDL_Rect({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y, 32, 32 }), nullptr, 0, PlayerAttack::P_Attack_Type::ARROW_SLOW), iPoint({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y }));
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

		return true;
	}
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
	

bool ArrowSlowItem::printIconOnScreen(iPoint pos)
{
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(SHEEP_ICON), 1, 0);

	return true;
}

Arrow_Slow_Shit::Arrow_Slow_Shit(Collider * temp, iPoint pos)
{
}

Arrow_Slow_Shit::~Arrow_Slow_Shit()
{
	App->colliders->deleteCollider(arrow_slow_collider);
}
