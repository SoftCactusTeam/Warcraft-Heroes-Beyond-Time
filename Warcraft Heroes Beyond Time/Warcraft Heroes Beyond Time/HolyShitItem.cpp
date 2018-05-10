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
}

bool HolyShitItem::Start()
{
	App->scene->player->numStats.hp += 100;
	App->scene->player->numStats.maxhp += 100;
	App->scene->player->numStats.damage += 70;
	
	return true;
}

bool HolyShitItem::Act(ModuleItems::ItemEvent event, float dt)
{
	return true;
}

bool HolyShitItem::Draw()
{
	SDL_Texture* toBlit = App->transitions->GetTexturebyRadius({ (int)App->scene->player->pos.x + 10 + App->render->camera.x,(int)App->scene->player->pos.y + 10 + App->render->camera.y },100, 640,360);
	App->printer->PrintSprite({ 0 - App->render->camera.x, 0 - App->render->camera.y}, toBlit, {0,0,640,360}, 10);
	SDL_DestroyTexture(toBlit);
	return true;
}

bool HolyShitItem::printIconOnScreen(iPoint pos)
{
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(HOLY_SHIT_ICON), 1, 0);
}
