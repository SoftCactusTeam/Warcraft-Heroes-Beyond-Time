#include "EnergyItem.h"
#include "Scene.h"
#include "Thrall.h"
#include "Application.h"
#include "ModuleRender.h"



EnergyItem::EnergyItem()
{
}


EnergyItem::~EnergyItem()
{
}

bool EnergyItem::Start()
{
	return true;
}

bool EnergyItem::Act(ModuleItems::ItemEvent event, float dt)
{
	switch (event)
	{
	case ModuleItems::ItemEvent::PLAYER_HITTED:
		App->scene->player->IncreaseEnergy(ModuleItems::energywhenHitted);
		break;
	}
	return true;
}

bool EnergyItem::Draw()
{
	return true;
}

bool EnergyItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(ENERGY_ITEM), 1, 0);
}
