#include "AngelsGuardItem.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModulePrinter.h"

bool AngelsGuardItem::Start()
{
	return true;
}

bool AngelsGuardItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
		case ModuleItems::ItemEvent::PLAYER_DIED:
		{
			App->scene->player->state = PlayerEntity::states::PL_RELIVE;
			App->items->unequipItem(this);
			App->audio->PlayFx(App->audio->Thrall_Revive);
			break;
		}
	}
	return true;
}

bool AngelsGuardItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	return true;
}

bool AngelsGuardItem::printYourStuff(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	iPoint iconPos = {171/2 - 32/2 ,50};
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(ANGEL_ICON), 1, 0);
	
	printMyString((char*)Title.data(), { 171 / 2 + pos.x, 100 + pos.y }, true);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 150 + pos.y });
	return true;
}

