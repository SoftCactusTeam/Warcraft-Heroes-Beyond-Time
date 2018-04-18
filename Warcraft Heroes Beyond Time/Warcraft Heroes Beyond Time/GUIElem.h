#ifndef __GUIELEM_H__
#define __GUIELEM_H__

#include "SDL\include\SDL_events.h"
#include "SDL\include\SDL_mouse.h"
#include "p2Point.h"
#include "ModuleGUI.h"
#include <list>

class Module;

enum class UIEvents
{
	NO_EVENT = -1,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_RIGHT_CLICK,
	MOUSE_LEFT_CLICK,
	MOUSE_RIGHT_UP,
	MOUSE_LEFT_UP,
	MAX_EVENTS
};

class GUIElem
{
	friend class ModuleGUI;
public:

	enum class GUIElemType
	{
		NO_ELEMTYPE = -1,
		BUTTON,
		LABEL,
		INPUTBOX,
		SLIDER,
		WINDOW,
		IMAGE,

		STAT,
		PLAYER_HP_BAR, 
		BOSS_HP_BAR
	};
	
protected:
	GUIElem* parent = nullptr;
	std::list<GUIElem*> childs;

	bool focused = false;

	fPoint localPos = { 0.0f, 0.0f };
	fPoint screenPos = { 0.0f, 0.0f };

	
	Module* listener = nullptr;
public:
	GUIElemType type = GUIElemType::NO_ELEMTYPE;
	UIEvents UIevent = UIEvents::NO_EVENT;
	SDL_Rect atlasRect = { 0, 0, 0, 0 };

public:
	GUIElem() {}
	GUIElem(fPoint localPos, Module* listener, SDL_Rect atlasRect, GUIElemType type, GUIElem* parent = nullptr);

	virtual ~GUIElem();

	virtual bool IsFocused()
	{
		return focused;
	}
	virtual void Focus()
	{
		focused = true;
	}
	virtual void UnFocus()
	{
		focused = false;
	}
	virtual void UnFocusChilds() {}

	virtual bool Update(float dt);
	virtual bool Draw();
	virtual bool MouseHover() const;
	virtual void DebugDraw();
	virtual bool HandleInput();

	bool hasParent()const;
	GUIElem* getParent() const;
	

	fPoint calculateScreenPos();

	void Move(fPoint dist);

protected:
	void addChild(GUIElem* child);
	bool UpdateChilds(float dt);
	bool DrawChilds();
	bool DestroyChilds();

};

#endif
