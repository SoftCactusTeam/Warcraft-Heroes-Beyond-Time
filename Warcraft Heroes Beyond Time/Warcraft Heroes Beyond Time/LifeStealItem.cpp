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

bool LifeStealItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(LIFE_STEAL_ICON), 1, 0);
}

