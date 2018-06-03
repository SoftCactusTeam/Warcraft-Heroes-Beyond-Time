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
		if(App->scene->player->numStats.hp > 0.1)
			App->scene->player->numStats.hp = 0.1;
		App->scene->player->numStats.damage = std::numeric_limits<float>::max();
		break;
	}
	return true;
}

bool OneShotItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	//HERE A CUTE ANIMATION IF U WANT
	return true;
}

bool OneShotItem::printYourStuff(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	iPoint iconPos = { 171 / 2 - 32 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(ONE_SHOT_ICON), 1, 0);
	printMyString((char*)Title.data(), { 171 / 2 + pos.x, 100 + pos.y }, true);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 150 + pos.y });
	return true;
}

const std::string OneShotItem::myNameIs() const
{
	return std::string(Title);
}

