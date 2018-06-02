#include "GUIWindow.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "GUIImage.h"
#include "Label.h"

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

	if (menu)
	{
		if (vertical)
			checkVerticalInputs(dt);
		else
			checkHorizontalInputs(dt);
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

GUIElem* GUIWindow::getTheFocused() const
{
	std::list<GUIElem*>::const_iterator focusedIt;
	for (focusedIt = childs.begin(); focusedIt != childs.end(); ++focusedIt)
	{
		if ((*focusedIt)->IsFocused())
			return *focusedIt;
	}
	return nullptr;
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
				if (!(*toFocusIt)->IsFocused() && ((*toFocusIt)->type == GUIElemType::BUTTON || (*toFocusIt)->type == GUIElemType::SLIDER || (*toFocusIt)->type == GUIElemType::ITEM_CONTAINER || (*toFocusIt)->type == GUIElemType::LABEL))
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
		if (focused->type == GUIElemType::LABEL)
			Label::waitingBindInput = false;
		focused->UnFocus();
	}

	if (toFocus != nullptr)
	{
		toFocus->Focus();
		if (toFocus->type == GUIElemType::LABEL)
			Label::waitingBindInput = true;
	}
}

void GUIWindow::FocusPrevChild()
{
	GUIElem* focused = nullptr;
	GUIElem* toFocus = nullptr;

	std::list<GUIElem*>::reverse_iterator focusedIt;
	for (focusedIt = childs.rbegin(); focusedIt != childs.rend(); ++focusedIt)
	{
		if ((*focusedIt)->IsFocused())
		{
			focused = (*focusedIt);

			std::list<GUIElem*>::reverse_iterator toFocusIt = focusedIt;
			while (toFocus == nullptr)
			{
				if (!(*toFocusIt)->IsFocused() && ((*toFocusIt)->type == GUIElemType::BUTTON || (*toFocusIt)->type == GUIElemType::SLIDER || (*toFocusIt)->type == GUIElemType::ITEM_CONTAINER || (*toFocusIt)->type == GUIElemType::LABEL))
					toFocus = *toFocusIt;
				if (std::next(toFocusIt) == childs.rend())
					toFocusIt = childs.rbegin();
				else
					std::advance(toFocusIt, 1);
			}
		}
	}

	if (focused != nullptr)
	{
		if (focused->type == GUIElemType::LABEL)
			Label::waitingBindInput = false;
		focused->UnFocus();
	}

	if (toFocus != nullptr)
	{
		toFocus->Focus();
		if (toFocus->type == GUIElemType::LABEL)
			Label::waitingBindInput = true;
	}
}

bool GUIWindow::checkVerticalInputs(float dt)
{
	if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_DOWN || App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
	{
		if (!AnyChildFocused())
		{
			childs.front()->Focus();
		}

		else if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_DOWN)
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

	if (App->input->GetAxis((int)Axis::UP) == KeyState::KEY_DOWN)
	{
		if (!AnyChildFocused())
		{
			childs.back()->Focus();
		}

		else
		{
			FocusPrevChild();
		}
	}

	else if (App->input->GetAxis((int)Axis::UP) == KeyState::KEY_REPEAT)
	{
		counter += dt;
		if (counter > 0.4)
		{
			minicounter += dt;
			if (minicounter > 0.2)
			{
				FocusPrevChild();
				minicounter = 0.0f;
			}
		}
	}

	if (App->input->GetAxis((int)Axis::DOWN) == KeyState::KEY_UP || App->input->GetAxis((int)Axis::UP) == KeyState::KEY_UP)
	{
		counter = 0;
		minicounter = 0;
	}

	return true;
}

bool GUIWindow::checkHorizontalInputs(float dt)
{
	GUIElem* elemfocused = getTheFocused();

	if (App->input->GetAxis((int)Axis::RIGHT) == KeyState::KEY_DOWN || App->input->GetPadButtonDown(SDL_CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
	{
		if (!AnyChildFocused())
		{
			childs.front()->Focus();
		}

		else if (App->input->GetAxis((int)Axis::RIGHT) == KeyState::KEY_DOWN && !elemfocused->AreYouPicking())
		{
			FocusNextChild();
		}
	}

	else if (App->input->GetAxis((int)Axis::RIGHT) == KeyState::KEY_REPEAT && elemfocused && !elemfocused->AreYouPicking())
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

	if (App->input->GetAxis((int)Axis::LEFT) == KeyState::KEY_DOWN)
	{
		if (!AnyChildFocused())
		{
			childs.back()->Focus();
		}

		else if(!getTheFocused()->AreYouPicking())
		{
			FocusPrevChild();
		}
	}

	else if (App->input->GetAxis((int)Axis::LEFT) == KeyState::KEY_REPEAT && elemfocused && !elemfocused->AreYouPicking())
	{
		counter += dt;
		if (counter > 0.4)
		{
			minicounter += dt;
			if (minicounter > 0.2)
			{
				FocusPrevChild();
				minicounter = 0.0f;
			}
		}
	}

	if (App->input->GetAxis((int)Axis::RIGHT) == KeyState::KEY_UP || App->input->GetAxis((int)Axis::LEFT) == KeyState::KEY_UP)
	{
		counter = 0;
		minicounter = 0;
	}
	return true;
}

void GUIWindow::setOpacity(Uint8 amount)
{
	std::list<GUIElem*>::iterator it;
	for (it = childs.begin(); it != childs.end(); ++it)
	{
		GUIElem* elem = *it;
		if (elem->type == GUIElemType::IMAGE)
		{
			GUIImage* image = (GUIImage*)elem;
			image->setOpacity(amount);
		}
	}
}