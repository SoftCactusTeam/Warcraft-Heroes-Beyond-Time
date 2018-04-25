#include "ItemContainer.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleItems.h"
#include "Scene.h"
#include "Item.h"

ItemContainer::ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::ITEM_CONTAINER, parent), item(item)
{
	anim.PushBack({368,336,53,53});
	anim.PushBack({496,298,77,90});
	anim.PushBack({496,167,101,128});
	anim.PushBack({368,167,125,166});
	anim.PushBack({ 42,167,171,242});
	anim.life = 1;
}

bool ItemContainer::Update(float dt)
{
	HandleInput(dt);

	if (focused )
	{
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
		{
			if (!selected)
				selected = true;
			else
			{
				App->items->equipItem(item);
				App->gui->DestroyElem((GUIElem*)App->scene->ItemSelection);
			}	
		}
	}

	return true;
}

bool ItemContainer::Draw()
{
	return true;
}