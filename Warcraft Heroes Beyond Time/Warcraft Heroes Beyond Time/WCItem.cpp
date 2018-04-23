#include "WCItem.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Scene.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"
#include "Thrall.h"

bool WCItem::Start()
{
	return true;
}

bool WCItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
	case ModuleItems::ItemEvent::UPDATE:
		time += dt;
		if (time >= 0.1)
		{
			time = 0;
			if (cont < 18)
			{
				wcpaper.push_front({ (int)App->scene->player->pos.x,(int)App->scene->player->pos.y });
				paper_collider.push_front(App->colliders->AddPlayerAttackCollider(SDL_Rect({(int)App->scene->player->pos.x, (int)App->scene->player->pos.y, 32, 32}), nullptr, 5, PlayerAttack::P_Attack_Type::SHIT));
				cont += 1;
			}
			if (cont == 18)
			{
				cont -= 1;
				wcpaper.pop_back();
				App->colliders->deleteCollider(paper_collider.back());
				paper_collider.pop_back();
			}
		}
		break;
	}
	return true;
}

bool WCItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	std::list<iPoint>::iterator it = wcpaper.begin();

	for (; it != wcpaper.end(); ++it)
	{
		App->printer->PrintSprite({ it->x, it->y }, App->items->getItemsTexture(), SDL_Rect(WC_ICON), -1);
	}

	return true;
}

bool WCItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(WC_ICON), 1, 0);
}


