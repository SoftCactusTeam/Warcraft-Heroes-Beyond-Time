#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "GUIElem.h"
#include "Log.h"

GUIElem::GUIElem(iPoint position, GUIElem* parent, Module* listener) : position(position), parent(parent), listener(listener)
{

}

GUIElem::~GUIElem() {}

bool GUIElem::Update(float dt)
{
	return true;
}

bool GUIElem::MouseHover() const //In progress
{
	int x, y;
	Application->input->GetMousePosition(x, y);
	//uint scale = Application->window->GetScale();

	return true;
}

void GUIElem::HandleInput()
{
	switch (UIevent) {

	case UIEvents::NO_EVENT:
		if (MouseHover()) {
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
		else if (Application->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED)
		{
			LOG("Mouse left cLick pressed");
			UIevent = UIEvents::MOUSE_LEFT_CLICK;
			listener->OnUIEvent((GUIElem*)this, UIevent);
			break;
		}
		else if (Application->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_PRESSED)
		{
			LOG("Mouse right click pressed");
			UIevent = UIEvents::MOUSE_RIGHT_CLICK;
			listener->OnUIEvent((GUIElem*)this, UIevent);
			break;
		}
		break;

	case UIEvents::MOUSE_RIGHT_CLICK:
		if (Application->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == SDL_RELEASED)
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
			UIevent = UIEvents::MOUSE_LEAVE;
			break;
		}
		else if (Application->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED)
		{
			LOG("Mouse left click released");
			listener->OnUIEvent((GUIElem*)this, UIevent);
			UIevent = MOUSE_ENTER;
			break;
		}

		break;
	case UIEvents::MOUSE_LEAVE:
		listener->OnUIEvent((GUIElem*)this, UIevent);
		UIevent = UIEvents::NO_EVENT;
		break;
	}
}

void GUIElem::DebugDraw() //In progress
{
	
}