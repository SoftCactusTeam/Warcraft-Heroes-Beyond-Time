#include "GUIWindow.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

GUIWindow::GUIWindow(fPoint localPos, SDL_Rect atlasRect, GUIElem* parent, Module* listener) : GUIElem(localPos, listener, atlasRect, GUIElemType::WINDOW, parent)
{
	background.x = 0;
	background.y = 0;
	background.w = 640;
	background.h = 360;
}

GUIWindow::~GUIWindow()
{
	DestroyChilds();
}

bool GUIWindow::Update(float dt)
{
	bool result = true;

	if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_DOWN)
	{
		if (!AnyChildFocused())
		{
			childs.front()->Focus();
			childs.front()->UIevent = UIEvents::MOUSE_ENTER;
		}
			
		else
		{
			FocusNextChild();
		}
	}

	else if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_REPEAT)
	{
		counter += dt;
		if (counter > 0.4)
		{
			minicounter += dt;
			if (minicounter > 0.2)
			{
				FocusNextChild();
				minicounter = 0.0f;
			}
		}
	}

	else if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_UP)
	{
		counter = 0;
		minicounter = 0;
	}
	
	if(result)
		result = UpdateChilds(dt);

	return result;
}

bool GUIWindow::Draw()
{
	bool result = true;

	background.x = -App->render->camera.x;
	background.y = -App->render->camera.y;

	if(blackBackground)
		result = App->render->DrawQuad(background, 0, 0, 0, 200, true, true);

	if (result)
		result = App->render->Blit(App->gui->getAtlas(), (int)(this->screenPos.x - App->render->camera.x), (int)(this->screenPos.y - App->render->camera.y), &atlasRect);
	
	if (result)
		result = DrawChilds();

	return result;

}

void GUIWindow::UnFocusChilds()
{
	std::list<GUIElem*>::iterator focusedIt;
	for (focusedIt = childs.begin(); focusedIt != childs.end(); ++focusedIt)
	{
		if ((*focusedIt)->IsFocused())
		{
			(*focusedIt)->UnFocus();
			(*focusedIt)->UIevent = UIEvents::MOUSE_LEAVE;
			break;
		}
	}
}

bool GUIWindow::AnyChildFocused()
{
	std::list<GUIElem*>::iterator focusedIt;
	for (focusedIt = childs.begin(); focusedIt != childs.end(); ++focusedIt)
	{
		if ((*focusedIt)->IsFocused())
			return true;
	}
	return false;
}

void GUIWindow::FocusNextChild()
{
	GUIElem* focused = nullptr;
	GUIElem* toFocus = nullptr;

	std::list<GUIElem*>::iterator focusedIt;
	for (focusedIt = childs.begin(); focusedIt != childs.end(); ++focusedIt)
	{
		if ((*focusedIt)->IsFocused())
		{
			focused = (*focusedIt);

			std::list<GUIElem*>::iterator toFocusIt = focusedIt;
			while (toFocus == nullptr)
			{
				if (!(*toFocusIt)->IsFocused() && ((*toFocusIt)->type == GUIElemType::BUTTON || (*toFocusIt)->type == GUIElemType::SLIDER))
					toFocus = *toFocusIt;
				if (std::next(toFocusIt) == childs.end())
					toFocusIt = childs.begin();
				else
					std::advance(toFocusIt, 1);
			}
		}
	}

	if (focused != nullptr)
	{
		focused->UnFocus();
		focused->UIevent = UIEvents::MOUSE_LEAVE;
	}

	if (toFocus != nullptr)
	{
		toFocus->Focus();
		toFocus->UIevent = UIEvents::MOUSE_ENTER;
	}
}