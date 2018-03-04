#ifndef __GUIELEM_H__
#define __GUIELEM_H__

#include "SDL\include\SDL_events.h"
#include "SDL\include\SDL_mouse.h"

#include "p2Point.h"
#include "ModuleGUI.h"

class Module;

enum UIEvents {
	NO_EVENT,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_RIGHT_CLICK,
	MOUSE_LEFT_CLICK,
	MOUSE_RIGHT_UP,
	MOUSE_LEFT_UP,
	MAX_EVENTS
};

enum ElementCentered //In progress
{
	NON_DEFINED,
	LEFT,
	CENTERED,
	RIGHT,
	MAX_DEFINED
};

class GUIElem
{
protected:
	GUIElem* parent = nullptr;
	Module* listener = nullptr;
	iPoint position = { 0,0 };
	SDL_Texture* textureToBlit = nullptr;
	UIEvents UIevent;

public:
	GUIElem() {}
	GUIElem(iPoint position, GUIElem* parent, Module* listener);

	virtual ~GUIElem();

	virtual bool Update(float dt);
	virtual bool MouseHover() const;
	virtual void DebugDraw();
	virtual void HandleInput();
};

#endif
