#include "ItemContainer.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleItems.h"
#include "Scene.h"
#include "Item.h"
#include "ModuleRender.h"
#include "Item.h"
#include "Log.h"

ItemContainer::ItemContainer(fPoint localPos, Module* listener, Item* item, GUIElem* parent) : GUIElem(localPos, listener, atlasRect, GUIElemType::ITEM_CONTAINER, parent), item(item)
{
	grow_anim.PushBack({ 368,336,53,53 }, { 26,26 });
	grow_anim.PushBack({ 496,298,77,90 }, { 38, 45 });
	grow_anim.PushBack({ 496,167,101,128 }, { 50, 64 });
	grow_anim.PushBack({ 368,167,125,166 }, { 62, 83 });
	grow_anim.PushBack({ 216,167,149,204 }, { 74, 102 });
	grow_anim.PushBack({ 42,167,171,242 }, {85, 121});
	grow_anim.life = 0.3f;
	grow_anim.loop = false;

	focused_anim.PushBack({ 805,153,195,270 }, {97,135});
	focused_anim.life = 0.1;

	select_anim.PushBack({ 615,167,171,242 }, { 85, 121 }); //White Filled
	select_anim.PushBack({ 42,167,171,242 }, { 85, 121 });	//Black Filled
	select_anim.life = 0.5f;
	select_anim.loop = false;

	anim = &grow_anim;
}

bool ItemContainer::Update(float dt)
{
	HandleInput(dt);

	if (focused)
	{
		if (anim == &grow_anim && anim->Finished())
			anim = &focused_anim;
		if (App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN && item != nullptr)
		{
			if (!selected)
			{
				selected = true;
				anim = &select_anim;
			}
		}
		else if (anim == &select_anim && select_anim.Finished())
		{
			App->items->equipItem(item);
			App->gui->DestroyElem((GUIElem*)App->scene->ItemSelection);
			App->scene->ItemSelection = nullptr;
			App->scene->paused = false;
		}
	}
	else
		selected = false;

	return true;
}

bool ItemContainer::HandleInput(float dt)
{
	bool ret = true;
	if (listener)
		switch (UIevent)
		{

		case UIEvents::NO_EVENT:

			if ((MouseHover() || focused))
			{
				parent->UnFocusChilds();
				Focus();
				UIevent = UIEvents::MOUSE_ENTER;
				listener->OnUIEvent((GUIElem*)this, UIevent);
				break;
			}
			break;

		case UIEvents::MOUSE_ENTER:

			if (focused && (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED || App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN))
			{
				LOG("Mouse left cLick pressed");
				UIevent = UIEvents::MOUSE_LEFT_CLICK;
				listener->OnUIEvent((GUIElem*)this, UIevent);
				break;
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED)
			{
				LOG("Mouse right click pressed");
				UIevent = UIEvents::MOUSE_RIGHT_CLICK;
				listener->OnUIEvent((GUIElem*)this, UIevent);
				break;
			}
			else if (!focused)
			{
				UIevent = UIEvents::MOUSE_LEAVE;
				listener->OnUIEvent((GUIElem*)this, UIevent);
			}
			break;

		case UIEvents::MOUSE_RIGHT_CLICK:
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED)
			{
				LOG("Mourse right click released");
				listener->OnUIEvent((GUIElem*)this, UIevent);
				UIevent = UIEvents::MOUSE_ENTER;
				break;
			}
			break;

		case UIEvents::MOUSE_LEFT_CLICK:

			if (focused && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED || App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_UP)
			{
				LOG("Mouse left click released");
				UIevent = UIEvents::MOUSE_LEFT_UP;
				ret = listener->OnUIEvent((GUIElem*)this, UIevent);
			}

			break;
		case UIEvents::MOUSE_LEFT_UP:
			if (focused == false)
				UIevent = UIEvents::MOUSE_LEAVE;
			else
				UIevent = UIEvents::MOUSE_ENTER;
			listener->OnUIEvent(this, UIevent);
			break;
		case UIEvents::MOUSE_LEAVE:
			listener->OnUIEvent((GUIElem*)this, UIevent);
			UIevent = UIEvents::NO_EVENT;
			break;
		}
	return ret;
}

bool ItemContainer::Draw()
{
	if(anim != &select_anim)
		App->render->Blit(App->gui->getAtlas(), localPos.x - anim->GetCurrentPivot().x, localPos.y - anim->GetCurrentPivot().y, &anim->GetCurrentFrame(App->dt), 1, 0);

	if (item != nullptr && (anim == &grow_anim && anim->Finished() || anim != &grow_anim))
		item->printIconOnScreen({ (int)localPos.x,(int)localPos.y });
	
	if (anim == &select_anim)
		App->render->Blit(App->gui->getAtlas(), localPos.x - anim->GetCurrentPivot().x, localPos.y - anim->GetCurrentPivot().y, &anim->GetCurrentFrame(App->dt), 1, 0);
	
	return true;
}