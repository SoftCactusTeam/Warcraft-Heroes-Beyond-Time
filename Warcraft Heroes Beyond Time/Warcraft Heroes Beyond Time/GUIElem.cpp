#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "GUIElem.h"
#include "Log.h"
#include "ModuleRender.h"

GUIElem::GUIElem(fPoint localPos, Module* listener, SDL_Rect atlasRect, GUIElemType type, GUIElem* parent) : localPos(localPos), listener(listener), atlasRect(atlasRect), type(type), parent(parent)
{
	calculateScreenPos();
}

GUIElem::~GUIElem() 
{
	DestroyChilds();
}

bool GUIElem::Update(float dt)
{
	return true;
}

bool GUIElem::Draw()
{
	return true;
}

bool GUIElem::MouseHover() const
{
	int x, y;
	App->input->GetMousePosition(x, y);
	uint scale = App->window->GetScale();

	bool result = false;

	//if collides
	if (!(x < screenPos.x ||
		x > screenPos.x + atlasRect.w ||
		y < screenPos.y ||
		y > screenPos.y + atlasRect.h))
	{
		result = true;
	}
	return result;
}

bool GUIElem::HandleInput()
{
	bool ret = true;

	switch (UIevent)
	{

	case UIEvents::NO_EVENT:
		if (MouseHover()) 
		{
			UIevent = UIEvents::MOUSE_ENTER;
			listener->OnUIEvent((GUIElem*)this, UIevent);
			break;
		}
		break;

	case UIEvents::MOUSE_ENTER:
		if (!MouseHover()) {
			LOG("Mouse Leave");
			UIevent = UIEvents::MOUSE_LEAVE;
			break;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED)
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
		if (!MouseHover())
		{
			LOG("Mouse Leave");
			UIevent = UIEvents::MOUSE_LEFT_UP;
			ret = listener->OnUIEvent((GUIElem*)this, UIevent);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED)
		{
			LOG("Mouse left click released");
			UIevent = UIEvents::MOUSE_LEFT_UP;
			ret = listener->OnUIEvent((GUIElem*)this, UIevent);
		}

		break;
	case UIEvents::MOUSE_LEFT_UP:
		if (!MouseHover())
			UIevent = UIEvents::NO_EVENT;
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

void GUIElem::DebugDraw() //In progress
{
	
}

bool GUIElem::hasParent() const
{
	return parent != nullptr;
}

GUIElem* GUIElem::getParent() const
{
	return parent;
}

void GUIElem::addChild(GUIElem* child)
{
	childs.push_back(child);
}

bool GUIElem::UpdateChilds(float dt)
{
	bool result = true;

	if (childs.size() > 0)
	{
		std::list<GUIElem*>::iterator it;
		
		for (it = childs.begin(); it != childs.end() && result; ++it)
		{
			result = (*it)->Update(dt);
		}
	}

	return result;
}

bool GUIElem::DrawChilds()
{
	bool result = true;

	if (childs.size() > 0)
	{
		std::list<GUIElem*>::iterator it;

		for (it = childs.begin(); it != childs.end() && result; ++it)
		{
			result = (*it)->Draw();
		}
	}

	return result;
}

bool GUIElem::DestroyChilds()
{
	if (childs.size() > 0)
	{
		std::list<GUIElem*>::iterator it;
		for (it = childs.begin(); it != childs.end(); ++it)
		{
			delete *it;
		}
		childs.clear();
	}
	return childs.size() <= 0;
}

fPoint GUIElem::calculateScreenPos()
{
	fPoint screenPos = {0.0f, 0.0f};

	for (GUIElem* elem = this; elem != nullptr; elem = elem->parent)
	{
		screenPos += elem->localPos;
	}
	this->screenPos = screenPos;

	return screenPos;
}

void GUIElem::Move(fPoint dist)
{
	std::list<GUIElem*>::iterator it;
	for (it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Move(dist);
	}
	this->localPos += dist;
	calculateScreenPos();
}