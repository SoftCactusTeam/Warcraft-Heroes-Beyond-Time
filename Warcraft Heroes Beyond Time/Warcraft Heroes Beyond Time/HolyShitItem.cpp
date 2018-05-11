#include "HolyShitItem.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Scene.h"
#include "Thrall.h"
#include "ModuleTransitions.h"
#include "ModulePrinter.h"


HolyShitItem::HolyShitItem()
{
}


HolyShitItem::~HolyShitItem()
{
	if (text != nullptr)
		SDL_DestroyTexture(text);
}

bool HolyShitItem::Start()
{
	App->scene->player->numStats.hp += ModuleItems::HolyShitExtraHP;
	App->scene->player->numStats.maxhp += ModuleItems::HolyShitExtraHP;
	App->scene->player->numStats.damage += ModuleItems::HolyShitExtraDamage;

	return true;
}

bool HolyShitItem::Act(ModuleItems::ItemEvent event, float dt)
{
	return true;
}

bool HolyShitItem::Draw()
{
	if (text != nullptr)
		SDL_DestroyTexture(text);
	text = App->transitions->GetTexturebyRadius({ (int)App->scene->player->pos.x + 10 + 20 + App->render->camera.x,(int)App->scene->player->pos.y + 10 + 20 + App->render->camera.y },100, 680,400);
	App->printer->PrintSprite({ -20 - App->render->camera.x, -20 - App->render->camera.y}, text, {0,0,680,400}, 10);
	return true;
}

bool HolyShitItem::printIconOnScreen(iPoint pos)
{
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(HOLY_SHIT_ICON), 1, 0);
}
