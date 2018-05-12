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

	text = App->transitions->GetTexturebyRadius({640, 360}, 100, 640 * 2, 360 * 2);

	return true;
}

bool HolyShitItem::Act(ModuleItems::ItemEvent event, float dt)
{
	return true;
}

bool HolyShitItem::Draw()
{
	App->printer->PrintSprite({ (int)(App->scene->player->pos.x - 640 + 15), (int)(App->scene->player->pos.y - 360 + 10)}, text, {0,0,640*2,360*2}, 10);
	return true;
}

bool HolyShitItem::printIconOnScreen(iPoint pos)
{
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, &SDL_Rect(HOLY_SHIT_ICON), 1, 0);
}
