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
				std::shared_ptr<Collider*> col = App->colliders->AddPlayerAttackCollider(SDL_Rect({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y, 32, 32 }), nullptr, 1 * dt, PlayerAttack::P_Attack_Type::SHIT).lock();
				Shit* shit_pointer = new Shit(*col, iPoint({ (int)App->scene->player->pos.x, (int)App->scene->player->pos.y }));
				shit_list.push_front(shit_pointer);
				cont += 1;
			}
			if (cont == 18)
			{
				cont -= 1;
				std::list<Shit*>::iterator it = shit_list.end();
				std::advance(it, -1);
				delete *it;
				shit_list.erase(it);

			}
		}
		break;
	}
	return true;
}

bool WCItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	std::list<Shit*>::iterator it = shit_list.begin();

	for (; it != shit_list.end(); ++it)
	{
		App->printer->PrintSprite((*it)->pos, App->items->getItemsTexture(), SDL_Rect(SHIT_ITEM), -1); 
	}

	return true;
}

bool WCItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(WC_ICON), 1, 0);
}

Shit::Shit(Collider* temp, iPoint pos) : paper_collider(temp), pos(pos)
{

}

Shit::~Shit()
{
	App->colliders->deleteCollider(paper_collider);
}

WCItem::~WCItem() 
{
	std::list<Shit*>::iterator it = shit_list.begin();
	for (; it != shit_list.end(); ++it)
	{
		delete *it;
	}

	shit_list.clear();
}