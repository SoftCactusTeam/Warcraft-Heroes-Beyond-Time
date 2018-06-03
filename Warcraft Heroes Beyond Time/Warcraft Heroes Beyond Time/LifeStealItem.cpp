#include "LifeStealItem.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Scene.h"
#include "Thrall.h"

LifeStealItem::LifeStealItem()
{
}


LifeStealItem::~LifeStealItem()
{
}

bool LifeStealItem::Start()
{
	return true;
}

bool LifeStealItem::Act(ModuleItems::ItemEvent event, float dt)
{
	switch (event)
	{
	case ModuleItems::ItemEvent::PLAYER_HIT:

		App->scene->player->Heal(ModuleItems::stealhp);
		break;
	}
	return true;
}

bool LifeStealItem::Draw()
{
	return true;
}

bool LifeStealItem::printYourStuff(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	iPoint iconPos = { 171 / 2 - 31 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(LIFE_STEAL_ICON), 1, 0);
	printMyString((char*)Title.data(), { 171 / 2 + pos.x, 100 + pos.y }, true);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 150 + pos.y });
	return true;
}

const std::string LifeStealItem::myNameIs() const
{
	return std::string(Title);
}

