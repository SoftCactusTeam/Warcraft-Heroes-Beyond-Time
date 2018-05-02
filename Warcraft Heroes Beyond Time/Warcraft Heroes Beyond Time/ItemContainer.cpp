#include "ItemContainer.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleItems.h"
#include "Scene.h"
#include "Item.h"
#include "ModuleRender.h"
#include "Item.h"

ItemContainer::ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::ITEM_CONTAINER, parent), item(item)
{
	anim.PushBack({ 368,336,53,53 }, { 26,26 });
	anim.PushBack({ 496,298,77,90 }, { 38, 45 });
	anim.PushBack({ 496,167,101,128 }, { 50, 64 });
	anim.PushBack({ 368,167,125,166 }, { 62, 83 });
	anim.PushBack({ 216,167,149,204 }, { 74, 102 });
	anim.PushBack({ 42,167,171,242 }, {85, 121});
	anim.life = 0.3;
	anim.loop = false;
}

bool ItemContainer::Update(float dt)
{
	HandleInput(dt);

	if (focused && anim.Finished())
	{
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN && item != nullptr)
		{
			if (!selected)
				selected = true;
			else
			{
				App->items->equipItem(item);
				App->gui->DestroyElem((GUIElem*)App->scene->ItemSelection);
				App->scene->ItemSelection = nullptr;
				App->scene->paused = false;
			}
		}
	}
	else
		selected = false;

	return true;
}

bool ItemContainer::Draw()
{
	App->render->Blit(App->gui->getAtlas(), localPos.x - anim.GetCurrentPivot().x, localPos.y - anim.GetCurrentPivot().y, &anim.GetCurrentFrame(App->dt), 1, 0);

	if (item != nullptr && anim.Finished())
		item->printIconOnScreen({ (int)localPos.x,(int)localPos.y });
	return true;
}