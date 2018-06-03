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

bool EnergyItem::printYourStuff(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	iPoint iconPos = { 171 / 2 - 31 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(ENERGY_ITEM), 1, 0);
	printMyString((char*)Title.data(), { 171 / 2 + pos.x, 100 + pos.y }, true);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 150 + pos.y });
	return true;
}

const std::string EnergyItem::myNameIs() const
{
	return std::string(Title);
}
