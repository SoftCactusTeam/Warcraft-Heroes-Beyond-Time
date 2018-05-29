#ifndef _ITEM_CLASS_
#define _ITEM_CLASS_

#include "ModuleItems.h"
#include "p2Point.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Fonts.h"

#include <string>

class Item
{
public:

	Item() {}
	virtual ~Item() {}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Act(ModuleItems::ItemEvent event, float dt = App->dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool printYourStuff(iPoint pos) 
	{
		return true;
	}

	virtual const std::string myNameIs() const = 0;
	

protected:
	inline virtual bool printMyString(char* customString, iPoint halfPos, bool title = false)
	{
		SDL_Texture* string = App->fonts->Print(customString, White, title ? App->fonts->getFontbyName("LifeCraft80") : App->fonts->getFontbyName("LifeCraft46"), 500);
		int w, h;
		SDL_QueryTexture(string, NULL, NULL, &w, &h);
		App->render->Blit(string, halfPos.x - w*0.3 / 2, halfPos.y, nullptr, 0.3, 0, 0, 0, 0, true);
		SDL_DestroyTexture(string);
		return true;
	}
};

#endif