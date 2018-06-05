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
	text = App->transitions->GetTexturebyRadius({640, 360}, 100, 640 * 2, 360 * 2);

	return true;
}

bool HolyShitItem::Act(ModuleItems::ItemEvent event, float dt)
{
	switch (event)
	{
		case ModuleItems::ItemEvent::UPDATE:
		{
			if (!alreadyAppliedStats && App->scene->player)
			{
				alreadyAppliedStats = true;
				App->scene->player->numStats.hp += ModuleItems::HolyShitExtraHP;
				App->scene->player->numStats.maxhp += ModuleItems::HolyShitExtraHP;
				App->scene->player->numStats.damage += ModuleItems::HolyShitExtraDamage;
			}
			break;
		}
	}
	
	return true;
}

bool HolyShitItem::Draw()
{
	App->printer->PrintSprite({ (int)(App->scene->player->pos.x - 640 + 15), (int)(App->scene->player->pos.y - 360 + 10)}, text, {0,0,640*2,360*2}, 10);
	return true;
}

bool HolyShitItem::printYourStuff(iPoint pos)
{
	iPoint iconPos = { 171 / 2 - 32 / 2 ,50 };
	App->render->Blit(App->items->getItemsTexture(), pos.x + iconPos.x, pos.y + iconPos.y, &SDL_Rect(HOLY_SHIT_ICON), 1, 0);
	printMyString((char*)Title.data(), { 171 / 2 + pos.x, 100 + pos.y }, true);
	printMyString((char*)softDescription.data(), { 171 / 2 + pos.x, 150 + pos.y });
	return true;
}

const std::string HolyShitItem::myNameIs() const
{
	return std::string(Title);
}
