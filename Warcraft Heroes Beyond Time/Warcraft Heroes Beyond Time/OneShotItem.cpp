#include "OneShotItem.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModulePrinter.h"

bool OneShotItem::Start()
{
	return true;
}

bool OneShotItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
	case ModuleItems::ItemEvent::UPDATE:
		App->scene->player->numStats.hp = item_hp;
		App->scene->player->numStats.damage = item_damage;
	}
	return true;
}

bool OneShotItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	//HERE A CUTE ANIMATION IF U WANT
	return true;
}

bool OneShotItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(ONE_SHOT_ICON), 1, 0);
}

