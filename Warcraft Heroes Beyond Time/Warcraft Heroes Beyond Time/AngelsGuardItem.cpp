#include "AngelsGuardItem.h"
#include "ModuleRender.h"
#include "ModuleColliders.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModulePrinter.h"

bool AngelsGuardItem::Start()
{
	App->items->revived = false;
	return true;
}

bool AngelsGuardItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	switch (event)
	{
	case ModuleItems::ItemEvent::UPDATE:
		if (App->scene->player->numStats.hp < 1.0f && !App->items->revived)
		{
			App->scene->player->numStats.hp = App->scene->player->numStats.maxhp;
			App->items->revived = true;
			//FALTA HACER LA ANIMCIÓN DE REVIVIR
		}
	}
	return true;
}

bool AngelsGuardItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	//PRINT SOME PREETY AURA IF U WANT
	return true;
}

bool AngelsGuardItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(ANGEL_ICON), 1, 0);
}

