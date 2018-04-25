#include "ItemContainer.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleItems.h"
#include "Scene.h"
#include "Item.h"

ItemContainer::ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::ITEM_CONTAINER, parent), item(item)
{}

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